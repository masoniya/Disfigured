#pragma once

#include "MouseClickControl.h"
#include "MouseMoveControl.h"
#include "ShaderProgram.h"
#include "Tool.h"


enum clipboardMode {copy, paste};

class ClipBoard : public Tool, public MouseClickControl, public MouseMoveControl
{
public:
	ClipBoard();

	void use() override;
	void unuse() override;

	bool isActive();
	bool shouldDrawImage();
	bool shouldDrawTempBox();

	int getMode();

	void increaseSize();
	void decreaseSize();

	//Load image to clipboard directly
	void loadImage(std::string path);

	void drawImage(ShaderProgram * program);
	void drawTempBox(ShaderProgram * program);

	//Change clipboard use mode
	void setCopyMode();
	void setPasteMode();

private:

	unsigned int vao;
	unsigned int vbo;

	unsigned int boxVao;
	unsigned int boxVbo;

	bool active;
	bool shouldDraw;
	bool shouldDrawTemp;

	int mode;

	//Used to scale image when pasting
	float size;

	unsigned int texture;

	//The point clicked on during copy mode
	double firstXPos;
	double firstYPos;

	//The point released on during copy mode
	double secondXPos;
	double secondYPos;

	//The point clicked during paste mode
	double centerXPos;
	double centerYPos;

	//The current cursor position
	double currentXPos;
	double currentYPos;

	//Copy area from the canvas
	void copyPixels();

	void handleMouseClick(int button, int action, double xPosition, double yPosition);
	void handleMouseMovement(double xPosition, double yPosition, double xPrevPosition, double yPrevPosition);
};
