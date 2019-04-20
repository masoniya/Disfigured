#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

#include "Pencil.h"
#include "Window.h"
#include "ColorBox.h"


Pencil::Pencil() :
	active(false),
	mouseButtonDown(false)
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}


void Pencil::use()
{
	active = true;
}

void Pencil::unuse()
{
	active = false;
}

void Pencil::drawLines(ShaderProgram *program)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, lineVertexData.size() * sizeof(float), lineVertexData.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	float size = 20.0f; //width and height by pixel

	program->use();
	program->setUniformVec3("color", ColorBox::foregroundColor);
	program->setUniformFloat("width", size / Window::width);
	program->setUniformFloat("height", size / Window::height);
	
	glBindVertexArray(vao);
	glDrawArrays(GL_LINES, 0, (GLsizei)lineVertexData.size() / 2);

	lineVertexData.clear();
}

bool Pencil::shouldDrawLines()
{
	return !lineVertexData.empty();
}

void Pencil::handleMouseClick(int button, int action, double xPosition, double yPosition)
{
	if (active) {
		if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
			mouseButtonDown = true;
			//draw a line on the click point
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

void Pencil::handleMouseMovement(double xPosition, double yPosition, double xPrevPosition, double yPrevPosition)
{
	if (active) {
		if (mouseButtonDown) {
			lineVertexData.push_back((float)xPosition);
			lineVertexData.push_back((float)yPosition);
			lineVertexData.push_back((float)xPrevPosition);
			lineVertexData.push_back((float)yPrevPosition);
		}
	}
}
