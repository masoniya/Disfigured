#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "Marker.h"
#include "Window.h"


Marker::Marker(ShaderProgram * program, float size) :
	Brush(program, size)
{
}


void Marker::draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, lineVertexData.size() * sizeof(float), lineVertexData.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	program->use();
	program->setUniformVec3("color", getColor());
	program->setUniformFloat("width", size / Window::width);
	program->setUniformFloat("height", size / Window::height);

	glBindVertexArray(vao);
	glDrawArrays(GL_LINES_ADJACENCY, 0, (GLsizei)lineVertexData.size() / 2);

	lineVertexData.clear();
}

void Marker::handleMouseClick(int button, int action, double xPosition, double yPosition)
{
	if (active) {
		if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
			mouseButtonDown = true;

			prevPrevXPos = xPosition;
			prevPrevYPos = yPosition;
		}
		if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE) {
			mouseButtonDown = false;
		}
	}
}

void Marker::handleMouseMovement(double xPosition, double yPosition, double xPrevPosition, double yPrevPosition)
{
	if (active) {
		if (mouseButtonDown) {
			//previous previous point
			lineVertexData.push_back((float)prevPrevXPos);
			lineVertexData.push_back((float)prevPrevYPos);

			//previous point
			lineVertexData.push_back((float)xPrevPosition);
			lineVertexData.push_back((float)yPrevPosition);

			//current point
			lineVertexData.push_back((float)xPosition);
			lineVertexData.push_back((float)yPosition);
			lineVertexData.push_back((float)xPosition);
			lineVertexData.push_back((float)yPosition);
			
			prevPrevXPos = xPrevPosition;
			prevPrevYPos = yPrevPosition;
		}
	}
}
