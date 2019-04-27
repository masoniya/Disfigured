#include <iostream>

#include <stb/stb_image.h>
#include <glad/glad.h>

#include "ColorBox.h"
#include "ShaderProgram.h"
#include "InputManager.h"
#include "CoordTransform.h"
#include "ToString.h"


static void windowFocusCallback(GLFWwindow* window, int focused);
static void mouseClickCallback(GLFWwindow * window, int button, int action, int mods);
static void mouseMoveCallback(GLFWwindow * window, double xPosition, double yPosition);

int ColorBox::width = 300;
int ColorBox::height = 60;

double ColorBox::mouseXPos;
double ColorBox::mouseYPos;

glm::vec3 ColorBox::foregroundColor = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 ColorBox::backgroundColor = glm::vec3(1.0f, 1.0f, 1.0f);

const float imageVertices[] = {
	-1.0, -1.0,  0.0, 0.0,
	-1.0,  1.0,  0.0, 1.0,
	 1.0, -1.0,  1.0, 0.0,
	 1.0,  1.0,  1.0, 1.0,
	 1.0, -1.0,  1.0, 0.0,
	-1.0,  1.0,  0.0, 1.0,
};

ColorBox::ColorBox()
{
	//create the window
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(width, height, "color box", nullptr, nullptr);

	if (window == nullptr) {
		throw std::runtime_error("Failed to create GLFW window");
	}

	glfwSetWindowFocusCallback(window, windowFocusCallback);
	glfwSetMouseButtonCallback(window, mouseClickCallback);
	glfwSetCursorPosCallback(window, mouseMoveCallback);

	glfwSetWindowPos(window, 200, 200);

	glfwMakeContextCurrent(window);
	glViewport(0, 0, width, height);

	glDrawBuffer(GL_FRONT);
	glReadBuffer(GL_FRONT);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	//load the texture
	uint32_t texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	stbi_set_flip_vertically_on_load(true);

	int width, height, numComponents;
	unsigned char* data = stbi_load("resources/textures/colors.png", &width, &height, &numComponents, 0);

	if (data != nullptr) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		throw std::runtime_error("Failed to load texture : resources/colors.png\n");
	}

	stbi_image_free(data);

	 
	//create vertex data
	GLuint vao;
	GLuint vbo;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(imageVertices), imageVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);


	//draw the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);


	//compile the shaders
	ShaderProgram* colorProgram = new ShaderProgram("shaders/imageShader.vert", "shaders/imageShader.frag");
	colorProgram->use();
	colorProgram->setUniformInt("image", 0);

	glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glFlush();
}

void ColorBox::setActive()
{
	glfwMakeContextCurrent(window);
}


static void windowFocusCallback(GLFWwindow* window, int focused)
{
	if (focused)
	{
		glfwMakeContextCurrent(window);
		glViewport(0, 0, ColorBox::width, ColorBox::height);
	}
	else
	{
		;
	}
}

void mouseClickCallback(GLFWwindow * window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
		unsigned char* pixel = new unsigned char[4];
		double xPos = 0, yPos = 0;
		normalizedScreenToScreenCoords(ColorBox::mouseXPos, ColorBox::mouseYPos, &xPos, &yPos, ColorBox::width, ColorBox::height);
		glReadPixels((int)xPos , ColorBox::height - (int)yPos, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
		ColorBox::foregroundColor = glm::vec3(pixel[0] / 255.0f, pixel[1] / 255.0f, pixel[2] / 255.0f);
	}

	else if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS) {
		unsigned char* pixel = new unsigned char[4];
		double xPos = 0, yPos = 0;
		normalizedScreenToScreenCoords(ColorBox::mouseXPos, ColorBox::mouseYPos, &xPos, &yPos, ColorBox::width, ColorBox::height);
		glReadPixels((int)xPos, ColorBox::height - (int)yPos, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
		ColorBox::backgroundColor = glm::vec3(pixel[0] / 255.0f, pixel[1] / 255.0f, pixel[2] / 255.0f);
	}
}

void mouseMoveCallback(GLFWwindow * window, double xPosition, double yPosition)
{
	double normalizedXPosition;
	double normalizedYPosition;

	screenToNormalizedScreenCoords(xPosition, yPosition, &normalizedXPosition, &normalizedYPosition, ColorBox::width, ColorBox::height);

	ColorBox::mouseXPos = normalizedXPosition;
	ColorBox::mouseYPos = normalizedYPosition;
}
