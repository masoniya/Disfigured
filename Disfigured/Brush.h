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

	virtual void drawLines();

	bool shouldDrawLines();

protected:

	virtual void handleMouseClick(int button, int action, double xPosition, double yPosition) override;
	virtual void handleMouseMovement(double xPosition, double yPosition, double xPrevPosition, double yPrevPosition) override;

	std::vector<float> lineVertexData;

	bool active;
	bool mouseButtonDown;

private:

	unsigned int vao;
	unsigned int vbo;

	ShaderProgram *program;
	ColorType colortype;

	float size;

	void construct(ShaderProgram * program, ColorType type, float size);
	glm::vec3 getColor();
};
