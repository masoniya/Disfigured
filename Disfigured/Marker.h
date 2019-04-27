#pragma once

#include "Brush.h"


//The marker uses a unique shader that requires the use of the previous 2 points to draw correctly
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
