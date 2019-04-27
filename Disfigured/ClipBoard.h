#pragma once

#include "MouseClickControl.h"
#include "MouseMoveControl.h"
#include "ShaderProgram.h"
#include "Tool.h"


enum clipboardMode {copy, paste};

class ClipBoard : public MouseClickControl, public Tool//, public MouseMoveControl
{
public:
	ClipBoard();

	void handleMouseClick(int button, int action, double xPosition, double yPosition);
	//void handleMouseMovement(double xPosition, double yPosition, double xPrevPosition, double yPrevPosition);

	void use() override;
	void unuse() override;

	bool isActive();
	bool shouldDrawImage();
	int getMode();
	void increaseSize();
	void decreaseSize();

	//Load image to clipboard directly
	void loadImage(std::string path);

	void drawImage(ShaderProgram * program);

	//Change clipboard use mode
	void setCopyMode();
	void setPasteMode();

private:

	unsigned int vao;
	unsigned int vbo;

	bool active;

	int mode;

	bool shouldDraw;

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

	//Copy area from the canvas
	void copyPixels();
};
