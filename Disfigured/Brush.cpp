#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

#include "Brush.h"
#include "Window.h"
#include "ColorBox.h"


Brush::Brush(ShaderProgram * program)
{
	construct(program, ColorType::foreground, 20.0f);
}

Brush::Brush(ShaderProgram * program, float size)
{
	construct(program, ColorType::foreground, size);
}

Brush::Brush(ShaderProgram * program, ColorType type)
{
	construct(program, type, 20.0f);
}

Brush::Brush(ShaderProgram * program, ColorType type, float size)
{
	construct(program, type, size);
}


void Brush::construct(ShaderProgram * program, ColorType type, float size)
{
	this->active = false;
	this->mouseButtonDown = false;
	this->program = program;
	this->colortype = type;
	this->size = size;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

glm::vec3 Brush::getColor()
{
	switch (colortype) {
		case ColorType::foreground : 
			return ColorBox::foregroundColor;
			break;

		case ColorType::background : 
			return ColorBox::backgroundColor;
			break;
		default :
			return ColorBox::foregroundColor;
			break;
	}
}

void Brush::use()
{
	active = true;
}

void Brush::unuse()
{
	active = false;
}

void Brush::drawLines()
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
	glDrawArrays(GL_LINES, 0, (GLsizei)lineVertexData.size() / 2);

	lineVertexData.clear();
}

bool Brush::shouldDrawLines()
{
	return !lineVertexData.empty();
}

void Brush::handleMouseClick(int button, int action, double xPosition, double yPosition)
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

void Brush::handleMouseMovement(double xPosition, double yPosition, double xPrevPosition, double yPrevPosition)
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
