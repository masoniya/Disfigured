#include <glfw/glfw3.h>

#include "AirBrush.h"


AirBrush::AirBrush(ShaderProgram * program, float size) :
	Brush(program, size)
{
}

void AirBrush::update()
{
	if (active) {
		if (mouseButtonDown) {
			//draw on the click point
			lineVertexData.push_back((float)currentXPos);
			lineVertexData.push_back((float)currentYPos);
			lineVertexData.push_back((float)currentXPos);
			lineVertexData.push_back((float)currentYPos);
		}
	}
}

void AirBrush::handleMouseClick(int button, int action, double xPosition, double yPosition)
{
	if (active) {
		if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
			mouseButtonDown = true;

			currentXPos = xPosition;
			currentYPos = yPosition;

			//draw on the click point
			lineVertexData.push_back((float)xPosition);
			lineVertexData.push_back((float)yPosition);
			lineVertexData.push_back((float)xPosition);
			lineVertexData.push_back((float)yPosition);
		}
		if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE) {
			mouseButtonDown = false;
		}
	}
}

void AirBrush::handleMouseMovement(double xPosition, double yPosition, double xPrevPosition, double yPrevPosition)
{
	if (active) {
		currentXPos = xPosition;
		currentYPos = yPosition;
	}
}
