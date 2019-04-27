#pragma once

#include "MouseClickControl.h"
#include "Tool.h"


class ColorPicker : public MouseClickControl, public Tool
{
public:
	ColorPicker();

	void use() override;

	void unuse() override;

	//Get the color of the clicked pixel
	void handleMouseClick(int button, int action, double xPosition, double yPosition) override;

private:
	
	bool active;
};
