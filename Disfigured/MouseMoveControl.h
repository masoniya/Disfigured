#pragma once

//Interface to get mouse motion input
class MouseMoveControl
{
public:
	virtual void handleMouseMovement(double xPosition, double yPosition, double xPrevPosition, double yPrevPosition) = 0;
};
