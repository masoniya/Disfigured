#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "AirBrush.h"
#include "Window.h"


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

void AirBrush::draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, lineVertexData.size() * sizeof(float), lineVertexData.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	program->use();
	program->setUniformVec3("color", getColor());
	program->setUniformFloat("width", size / Window::width);
	program->setUniformFloat("height", size / Window::height);
	program->setUniformFloat("seed", (float)sin(glfwGetTime()));

	glBindVertexArray(vao);
	glDrawArrays(GL_POINTS, 0, (GLsizei)lineVertexData.size() / 2);

	lineVertexData.clear();
}

void AirBrush::handleMouseClick(int button, int action, double xPosition, double yPosition)
{
	if (active) {
		if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
			mouseButtonDown = true;

			currentXPos = xPosition;
			currentYPos = yPosition;
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
