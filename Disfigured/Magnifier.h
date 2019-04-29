#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "Tool.h"
#include "MouseClickControl.h"
#include "MouseMoveControl.h"
#include "ShaderProgram.h"


class Magnifier : public Tool, public MouseClickControl, public MouseMoveControl
{
public:
	Magnifier();

	void use() override;
	void unuse() override;

	std::vector<float> getVertexData();

	bool shouldDrawTempBox();

	void drawTempBox(ShaderProgram * program);

	bool shouldChangeZoom;
	bool zoomedIn;

	static glm::mat4 transform;

private:

	unsigned int boxVao;
	unsigned int boxVbo;

	double firstXPos;
	double firstYPos;

	double secondXPos;
	double secondYPos;

	double currentXPos;
	double currentYPos;

	bool active;
	bool shouldDrawTemp;

	void handleMouseClick(int button, int action, double xPosition, double yPosition) override;
	void handleMouseMovement(double xPosition, double yPosition, double xPrevPosition, double yPrevPosition) override;
};
