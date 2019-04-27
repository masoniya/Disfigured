#pragma once

#include "MouseClickControl.h"
#include "ShaderProgram.h"
#include "Tool.h"


//The filltool uses a stack-based floodfill algorithm
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

	//compare colors
	inline bool colors_equal(unsigned char(&first)[4], unsigned char(&second)[4]);

	//float to byte (for colors 0.0f-1.0f -> 0-255)
	inline char ftob(float fColor);

	bool active;

	bool shouldDraw;
	
	unsigned int texture;

	unsigned char* pixels;
};
