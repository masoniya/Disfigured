#pragma once

#include "Brush.h"


class AirBrush : public Brush
{
public:
	AirBrush(ShaderProgram * program, float size);

	void update();

protected:
	void handleMouseClick(int button, int action, double xPosition, double yPosition) override;
	void handleMouseMovement(double xPosition, double yPosition, double xPrevPosition, double yPrevPosition) override;

	double currentXPos;
	double currentYPos;
};
