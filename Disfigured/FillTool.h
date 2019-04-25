#pragma once

#include "MouseClickControl.h"
#include "ShaderProgram.h"
#include "Tool.h"


class FillTool : public MouseClickControl, public Tool
{
public:
	FillTool();

	void use() override;

	void unuse() override;

	void drawFrame(ShaderProgram* program, unsigned int vao);

	bool shouldDrawFrame();

private:

	void handleMouseClick(int button, int action, double xPosition, double yPosition) override;

	bool fill_Color(int xPos, int yPos, unsigned char(&fillColor)[4], unsigned char(&destColor)[4], int width, int height, unsigned char * pixels);

	inline bool colors_equal(unsigned char(&first)[4], unsigned char(&second)[4]);
	inline char ftob(float fColor);

	bool active;

	bool shouldDraw;
	
	unsigned int texture;

	unsigned char* pixels;
};
