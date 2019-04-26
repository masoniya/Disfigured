#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <stb/stb_image.h>

#include "ClipBoard.h"
#include "Window.h"
#include "CoordTransform.h"


ClipBoard::ClipBoard() :
	active(false),
	shouldDraw(false),
	texture(0),
	mode(copy)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//vertex data for drawing image
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}


void ClipBoard::use()
{
	active = true;
}

void ClipBoard::unuse()
{
	active = false;
}

bool ClipBoard::isActive()
{
	return active;
}

bool ClipBoard::shouldDrawImage()
{
	return shouldDraw;
}

int ClipBoard::getMode()
{
	return mode;
}

void ClipBoard::loadImage(std::string path)
{
	stbi_set_flip_vertically_on_load(true);

	int width, height, numComponents;
	unsigned char *data = stbi_load(path.c_str(), &width, &height, &numComponents, 0);

	GLenum format;

	if (numComponents == 1) {
		format = GL_RED;
	}
	else if (numComponents == 3) {
		format = GL_RGB;
	}
	else if (numComponents == 4) {
		format = GL_RGBA;
	}
	else {
		throw std::runtime_error("Unknown color format for texture : " + path + "\n");
	}

	if (data != nullptr) {
		glBindTexture(GL_TEXTURE_2D, texture);
		//update the contents of texture
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		//update the size of the printed image
		int x = 0, y = 0, x2 = width, y2 = height;

		screenToNormalizedScreenCoords(x, y, &firstXPos, &firstYPos, Window::width, Window::height);
		screenToNormalizedScreenCoords(x2, y2, &secondXPos, &secondYPos, Window::width, Window::height);
	}
	else {
		throw std::runtime_error("Failed to load texture : " + path + "\n");
	}

	stbi_image_free(data);
}

void ClipBoard::drawImage(ShaderProgram * program)
{
	float width, height, xPos, yPos;

	width = (float)abs(firstXPos - secondXPos) / 2;
	height = (float)abs(firstYPos - secondYPos) / 2;

	xPos = (float)centerXPos;
	yPos = (float)centerYPos;

	float vertices[] = {
		(xPos - width), (yPos - height),  0.0f, 0.0f, //bottom left
		(xPos - width), (yPos + height),  0.0f, 1.0f, //top left
		(xPos + width), (yPos - height),  1.0f, 0.0f, //bottom right
		(xPos + width), (yPos + height),  1.0f, 1.0f, //top right
		(xPos + width), (yPos - height),  1.0f, 0.0f, //bottom right
		(xPos - width), (yPos + height),  0.0f, 1.0f, //top left
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	program->use();
	program->setUniformInt("image", 0);

	glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	shouldDraw = false;
}

void ClipBoard::setCopyMode()
{
	//std::cout << "copy mode" << std::endl;
	mode = copy;
}

void ClipBoard::setPasteMode()
{
	//std::cout << "paste mode" << std::endl;
	mode = paste;
}

void ClipBoard::copyPixels()
{
	unsigned char * pixels;
	int x, y, x2, y2, width, height;

	//get appropriate coordinates
	normalizedScreenToScreenCoords(firstXPos, firstYPos, &x, &y, Window::width, Window::height);
	y = Window::height - y;

	normalizedScreenToScreenCoords(secondXPos, secondYPos, &x2, &y2, Window::width, Window::height);
	y2 = Window::height - y2;

	//calculate bottom left and top right points and clamp coords to window boundaries
	int bottomLeftX = (int)fmin(x, x2);
	//bottomLeftX = (int)fmax(bottomLeftX, 0);

	int bottomLeftY = (int)fmin(y, y2);
	//bottomLeftY = (int)fmax(bottomLeftY, 0);

	int topRightX = (int)fmax(x, x2);
	//topRightX = (int)fmin(topRightX, Window::width);

	int topRightY = (int)fmax(y, y2);
	//topRightY = (int)fmin(topRightY, Window::height);

	//calculate width and height
	width = abs(bottomLeftX - topRightX);
	height = abs(bottomLeftY - topRightY);

	//calculate padding (only with pack alingment of 4)
	/*int xPad = (4 - width % 4) % 4;
	std::cout << "pad : " << xPad << std::endl;*/

	//RGB because copied area should be opaque (don't forget to pad if not using 1 byte unpack alignment)
	pixels = new unsigned char[width * height * 3];
	glReadPixels(bottomLeftX, bottomLeftY, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	glBindTexture(GL_TEXTURE_2D, texture);	
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	
	delete (pixels);
}

void ClipBoard::handleMouseClick(int button, int action, double xPosition, double yPosition)
{
	if (!shouldDrawImage() && active && mode == copy) {
		if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
			firstXPos = xPosition;
			firstYPos = yPosition;
			//std::cout << "clicked xPos : " << xPosition << " yPos : " << yPosition << std::endl;
		}
		if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE) {
			//button release can happen outside window boundaries so we have to clamp the coords
			secondXPos = glm::clamp(xPosition, -1.0, 1.0);
			secondYPos = glm::clamp(yPosition, -1.0, 1.0);
			//std::cout << "released xPos : " << xPosition << " yPos : " << yPosition << std::endl;
			copyPixels();
		}
	}

	if (!shouldDrawImage() && active && mode == paste) {
		if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
			centerXPos = xPosition;
			centerYPos = yPosition;
			shouldDraw = true;
			//std::cout << "pasted xPos : " << xPosition << " yPos : " << yPosition << std::endl;
		}
	}
}
