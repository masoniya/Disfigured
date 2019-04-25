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

	void drawImage(ShaderProgram * program);

	void setCopyMode();
	void setPasteMode();

private:

	unsigned int vao;
	unsigned int vbo;

	bool active;
	int mode;

	bool shouldDraw;

	unsigned int texture;

	double firstXPos;
	double firstYPos;

	double secondXPos;
	double secondYPos;

	double centerXPos;
	double centerYPos;

	void copyPixels();
};
