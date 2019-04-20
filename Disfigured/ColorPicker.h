#pragma once

#include "MouseClickControl.h"


class ColorPicker : public MouseClickControl
{
public:
	ColorPicker();

	void use();

	void unuse();

	void handleMouseClick(int button, int action, double xPosition, double yPosition) override;

private:
	
	bool active;
};
