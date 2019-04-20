#pragma once

class MouseMoveControl
{
public:
	virtual void handleMouseMovement(double xPosition, double yPosition, double xPrevPosition, double yPrevPosition) = 0;
};
