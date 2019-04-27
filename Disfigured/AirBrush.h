#pragma once

#include "Brush.h"


//The airbrush draws points rather than lines and is updated every frame while the mouse is held
class AirBrush : public Brush
{
public:
	AirBrush(ShaderProgram * program, float size);

	//If the mouse button is down draw again
	void update();

	void draw() override;

protected:

	void handleMouseClick(int button, int action, double xPosition, double yPosition) override;
	void handleMouseMovement(double xPosition, double yPosition, double xPrevPosition, double yPrevPosition) override;

	double currentXPos;
	double currentYPos;
};
