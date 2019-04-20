#include <iostream>
#include <stack>

#include <glad/glad.h>

#include "FillTool.h"
#include "Window.h"
#include "CoordinateTransform.h"
#include "ColorBox.h"


struct Coords {
	int x;
	int y;

	Coords(int x, int y) {
		this->x = x;
		this->y = y;
	}
};


FillTool::FillTool() :
	active(false),
	shouldDraw(false),
	texture(0)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


void FillTool::use()
{
	active = true;
}

void FillTool::unuse()
{
	active = false;
}

void FillTool::drawFrame(ShaderProgram * program, unsigned int vao)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	program->use();
	program->setUniformInt("image", 0);

	glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	shouldDraw = false;
}

bool FillTool::shouldDrawFrame()
{
	return shouldDraw;
}

void FillTool::handleMouseClick(int button, int action, double xPosition, double yPosition)
{
	if (active) {
		if (!shouldDrawFrame() && button == GLFW_MOUSE_BUTTON_3 && action == GLFW_PRESS) {

			pixels = new unsigned char[Window::width * Window::height * 4];
			glReadPixels(0, 0, Window::width, Window::height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

			double xPos = 0, yPos = 0;
			normalizedScreenToScreenCoords(xPosition, yPosition, &xPos, &yPos, Window::width, Window::height);
			yPos = Window::height - yPos;

			int xIndex = (int)xPos;
			int yIndex = (int)yPos;

			unsigned char destColor[4];
			for (int i = 0; i < 4; i++) {
				destColor[i] = pixels[4 * (yIndex * Window::width + xIndex) + i];
			}

			unsigned char fillColor[4];
			fillColor[0] = ftob(ColorBox::foregroundColor.x);
			fillColor[1] = ftob(ColorBox::foregroundColor.y);
			fillColor[2] = ftob(ColorBox::foregroundColor.z);
			fillColor[3] = 255; //no alpha values currently


			//apply the fill algorithm (software rendering)
			std::stack<Coords> coordStack;
			coordStack.push(Coords(xIndex, yIndex));
			while (!coordStack.empty()) {
				Coords current = coordStack.top();
				coordStack.pop();

				//if it worked add it's neighbors
				if (fill_Color(current.x, current.y, fillColor, destColor, Window::width, Window::height, pixels)) {
					//4 way fill
					coordStack.push(Coords(current.x + 1, current.y));
					coordStack.push(Coords(current.x - 1, current.y));
					coordStack.push(Coords(current.x, current.y + 1));
					coordStack.push(Coords(current.x, current.y - 1));

					//8 way fill
					//coordStack.push(Coords(current.x + 1, current.y + 1));
					//coordStack.push(Coords(current.x + 1, current.y - 1));
					//coordStack.push(Coords(current.x - 1, current.y + 1));
					//coordStack.push(Coords(current.x - 1, current.y - 1));
				}
				//otherwise continue
			}

			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Window::width, Window::height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

			delete (pixels);

			shouldDraw = true;
		}
	}
}

bool FillTool::fill_Color(int xPos, int yPos, unsigned char (&fillColor)[4], unsigned char (&destColor)[4], int width, int height, unsigned char * pixels)
{
	//clicked the same color as the fill color
	if (colors_equal(fillColor, destColor)) {
		return 0;
	}

	//coords out of bounds
	if (xPos < 0 || xPos > width || yPos < 0 || yPos > height) {
		return 0;
	}

	//get the current pixel color
	unsigned char currentColor[4];
	for (int i = 0; i < 4; i++) {
		currentColor[i] = pixels[4 * (yPos * width + xPos) + i];
	}

	//change the current pixel
	if (colors_equal(destColor, currentColor)) {

		//copy the color
		for (int i = 0; i < 4; i++) {
			pixels[4 * (yPos * width + xPos) + i] = fillColor[i];
		}
		return 1;
	}
	return 0;
}

bool FillTool::colors_equal(unsigned char(&first)[4], unsigned char(&second)[4])
{
	for (int i = 0; i < 4; i++) {
		if (first[i] != second[i]) {
			return 0;
		}
	}
	return 1;
}

char FillTool::ftob(float fColor)
{
	return fColor >= 1.0f ? 255 : (int)(fColor * 256);
}
