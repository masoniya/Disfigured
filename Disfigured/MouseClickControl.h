#pragma once

class MouseClickControl
{
public:
	virtual void handleMouseClick(int button, int action, double xPosition, double yPosition) = 0;
};
