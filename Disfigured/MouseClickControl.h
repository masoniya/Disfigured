#pragma once

//Interface to get mouse click input
class MouseClickControl
{
public:
	virtual void handleMouseClick(int button, int action, double xPosition, double yPosition) = 0;
};
