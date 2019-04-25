#pragma once

#include "Brush.h"


class Marker : public Brush
{
public:
	Marker(ShaderProgram * program, float size);

	void draw() override;

protected:
	void handleMouseClick(int button, int action, double xPosition, double yPosition) override;
	void handleMouseMovement(double xPosition, double yPosition, double xPrevPosition, double yPrevPosition) override;

	double prevPrevXPos;
	double prevPrevYPos;
};
