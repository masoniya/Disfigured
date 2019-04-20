#pragma once

#include <vector>

#include "MouseClickControl.h"
#include "MouseMoveControl.h"
#include "ShaderProgram.h"


class Pencil : public MouseClickControl, public MouseMoveControl
{
public:
	Pencil();

	void use();

	void unuse();

	void drawLines(ShaderProgram *program);

	bool shouldDrawLines();

private:
	GLuint vao;
	GLuint vbo;

	void handleMouseClick(int button, int action, double xPosition, double yPosition) override;
	void handleMouseMovement(double xPosition, double yPosition, double xPrevPosition, double yPrevPosition) override;

	bool active;
	bool mouseButtonDown;

	std::vector<float> lineVertexData;
};
