#pragma once

//Interface to get mousewheel scroll input
class MouseScrollControl
{
public:
	virtual void handleMouseScroll(double xOffset, double yOffset) = 0;
};
