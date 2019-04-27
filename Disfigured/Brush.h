#pragma once

#include <vector>

#include "ShaderProgram.h"
#include "MouseClickControl.h"
#include "MouseMoveControl.h"


enum class ColorType {foreground, background};

//Base class for all brushes 
//Allows overriding of input control and drawing behavior
class Brush : public MouseClickControl, public MouseMoveControl
{
public:
	Brush(ShaderProgram * program);
	Brush(ShaderProgram * program, float size);
	Brush(ShaderProgram * program, ColorType type);
	Brush(ShaderProgram * program, ColorType type, float size);

	void use();

	void unuse();

	bool isActive();

	void increaseSize();

	void decreaseSize();

	//Draw all lines
	virtual void draw();

	bool shouldDrawLines();

protected:

	unsigned int vao;
	unsigned int vbo;

	ShaderProgram *program;
	ColorType colortype;
	float size;

	//Vector that contains vertex data of lines to be drawn
	std::vector<float> lineVertexData;

	bool active;
	bool mouseButtonDown;

	virtual void handleMouseClick(int button, int action, double xPosition, double yPosition) override;
	virtual void handleMouseMovement(double xPosition, double yPosition, double xPrevPosition, double yPrevPosition) override;

	void construct(ShaderProgram * program, ColorType type, float size);
	glm::vec3 getColor();
};
