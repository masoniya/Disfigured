#pragma once

#include <vector>

#include "ShaderProgram.h"
#include "MouseClickControl.h"
#include "MouseMoveControl.h"


enum class ColorType {foreground, background};

class Brush : public MouseClickControl, public MouseMoveControl
{
public:
	Brush(ShaderProgram * program);
	Brush(ShaderProgram * program, float size);
	Brush(ShaderProgram * program, ColorType type);
	Brush(ShaderProgram * program, ColorType type, float size);

	void use();

	void unuse();

	void drawLines();

	bool shouldDrawLines();

private:

	GLuint vao;
	GLuint vbo;

	ShaderProgram *program;
	ColorType colortype;

	float size;

	bool active;
	bool mouseButtonDown;

	std::vector<float> lineVertexData;

	void construct(ShaderProgram * program, ColorType type, float size);
	glm::vec3 getColor();

	void handleMouseClick(int button, int action, double xPosition, double yPosition) override;
	void handleMouseMovement(double xPosition, double yPosition, double xPrevPosition, double yPrevPosition) override;
};
