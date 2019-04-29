#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Magnifier.h"
#include "CoordTransform.h"
#include "Window.h"


const float defaultVertices[] = {
	-1.0f, -1.0f,  0.0f, 0.0f, //bottom left
	-1.0f,  1.0f,  0.0f, 1.0f, //top left
	 1.0f, -1.0f,  1.0f, 0.0f, //bottom right
	 1.0f,  1.0f,  1.0f, 1.0f, //top right
	 1.0f, -1.0f,  1.0f, 0.0f, //bottom right
	-1.0f,  1.0f,  0.0f, 1.0f, //top left
};

const glm::mat4 defaultTransform = glm::mat4(1.0f);

glm::mat4 Magnifier::transform = glm::mat4(1.0f);

Magnifier::Magnifier() :
	active(false),
	shouldDrawTemp(false),
	shouldChangeZoom(false),
	zoomedIn(false)
{
	//vertex data for drawing box outline
	glGenVertexArrays(1, &boxVao);
	glGenBuffers(1, &boxVbo);

	glBindVertexArray(boxVao);

	glBindBuffer(GL_ARRAY_BUFFER, boxVbo);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}


void Magnifier::use()
{
	active = true;
}

void Magnifier::unuse()
{
	active = false;
}

std::vector<float> Magnifier::getVertexData()
{
	if (!zoomedIn) {
		std::vector<float> defaultVertexData;
		for (int i = 0; i < sizeof(defaultVertices) / sizeof(defaultVertices[0]); i++) {
			defaultVertexData.push_back(defaultVertices[i]);
		}
		transform = defaultTransform;

		transform = glm::mat4(1.0f);

		return defaultVertexData;
	}
	
	//vertex positions
	float bottomLeftX = -1.0f;
	float bottomLeftY = -1.0f;
	float topLeftX = -1.0f;
	float topLeftY = 1.0f;
	float bottomRightX = 1.0f;
	float bottomRightY = -1.0f;
	float topRightX = 1.0f;
	float topRightY = 1.0f;
	
	//vertex texture coordinates
	float bottomLeftTexX;
	float bottomLeftTexY;
	float topLeftTexX;
	float topLeftTexY;
	float bottomRightTexX;
	float bottomRightTexY;
	float topRightTexX;
	float topRightTexY;

	//click point positions
	float blX = (float)fmin(firstXPos, secondXPos);
	float blY = (float)fmin(firstYPos, secondYPos);
	float tlX = (float)fmin(firstXPos, secondXPos);
	float tlY = (float)fmax(firstYPos, secondYPos);
	float brX = (float)fmax(firstXPos, secondXPos);
	float brY = (float)fmin(firstYPos, secondYPos);
	float trX = (float)fmax(firstXPos, secondXPos);
	float trY = (float)fmax(firstYPos, secondYPos);

	normalizedScreenToTexCoords(blX, blY, &bottomLeftTexX, &bottomLeftTexY);
	normalizedScreenToTexCoords(tlX, tlY, &topLeftTexX, &topLeftTexY);
	normalizedScreenToTexCoords(brX, brY, &bottomRightTexX, &bottomRightTexY);
	normalizedScreenToTexCoords(trX, trY, &topRightTexX, &topRightTexY);

	std::vector<float> vertices;
	vertices.push_back(bottomLeftX);
	vertices.push_back(bottomLeftY);
	vertices.push_back(bottomLeftTexX);
	vertices.push_back(bottomLeftTexY);

	vertices.push_back(topLeftX);
	vertices.push_back(topLeftY);
	vertices.push_back(topLeftTexX);
	vertices.push_back(topLeftTexY);

	vertices.push_back(bottomRightX);
	vertices.push_back(bottomRightY);
	vertices.push_back(bottomRightTexX);
	vertices.push_back(bottomRightTexY);

	vertices.push_back(topRightX);
	vertices.push_back(topRightY);
	vertices.push_back(topRightTexX);
	vertices.push_back(topRightTexY);

	vertices.push_back(bottomRightX);
	vertices.push_back(bottomRightY);
	vertices.push_back(bottomRightTexX);
	vertices.push_back(bottomRightTexY);

	vertices.push_back(topLeftX);
	vertices.push_back(topLeftY);
	vertices.push_back(topLeftTexX);
	vertices.push_back(topLeftTexY);

	float xOffset;
	float yOffset;

	normalizedScreenToScreenCoords(tlX , tlY, &xOffset, &yOffset, Window::width, Window::height);

	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(xOffset, yOffset, 0.0f));
	transform = glm::scale(transform, glm::vec3(abs(firstXPos - secondXPos) / 2.0f, abs(firstYPos - secondYPos) / 2.0f, 1.0f));

	return vertices;
}

bool Magnifier::shouldDrawTempBox()
{
	return shouldDrawTemp;
}

void Magnifier::drawTempBox(ShaderProgram * program)
{
	float vertices[] = {
	(float)firstXPos, (float)firstYPos, (float)currentXPos, (float)currentYPos
	};

	glBindBuffer(GL_ARRAY_BUFFER, boxVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	const float boxColor[] = { 0.0f, 0.0f, 0.0f };

	program->use();
	program->setUniformVec3("color", boxColor);

	glBindVertexArray(boxVao);

	glDrawArrays(GL_LINES, 0, 6);
}

void Magnifier::handleMouseClick(int button, int action, double xPosition, double yPosition)
{
	if (active) {
		if (zoomedIn) {
			if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS) {
				std::cout << "zooming out" << std::endl;

				shouldChangeZoom = true;
				zoomedIn = false;
			}
		}
		else {
			if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
				firstXPos = xPosition;
				firstYPos = yPosition;
				shouldDrawTemp = true;
				//std::cout << "clicked xPos : " << xPosition << " yPos : " << yPosition << std::endl;
			}
			if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE) {
				secondXPos = glm::clamp(xPosition, -1.0, 1.0);
				secondYPos = glm::clamp(yPosition, -1.0, 1.0);
				shouldDrawTemp = false;
				//std::cout << "released xPos : " << xPosition << " yPos : " << yPosition << std::endl;

				std::cout << "zooming in" << std::endl;
				
				shouldChangeZoom = true;
				zoomedIn = true;
			}
		}
	}
}

void Magnifier::handleMouseMovement(double xPosition, double yPosition, double xPrevPosition, double yPrevPosition)
{
	if (active) {
		currentXPos = glm::clamp(xPosition, -1.0, 1.0);
		currentYPos = glm::clamp(yPosition, -1.0, 1.0);
	}
}
