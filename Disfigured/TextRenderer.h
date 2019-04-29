#pragma once

#include <string>
#include <map>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "ShaderProgram.h"
#include "Tool.h"
#include "MouseClickControl.h"
#include "KeyboardControl.h"
#include "MouseMoveControl.h"


struct Character {
	GLuint texture;
	glm::ivec2 size;
	glm::ivec2 bearing;
	GLuint advance;
};


class TextRenderer : public Tool, public MouseMoveControl, public MouseClickControl, public KeyboardControl
{
public:
	TextRenderer(std::string fontPath);

	void renderText(ShaderProgram * program);
	void drawTempBox(ShaderProgram * program);

	void use() override;
	void unuse() override;

	bool isActive();
	bool isInputMode();

	void increaseSize();
	void decreaseSize();

	bool shouldDrawText();
	bool shouldDrawTempBox();

	void recalculateProjection();

private:

	unsigned int vao;
	unsigned int vbo;

	unsigned int boxVao;
	unsigned int boxVbo;

	int characterHeight;
	int lineSpacing;

	//click point
	double x;
	double y;

	//release point
	double x2;
	double y2;

	//top left point
	float topLeftX;
	float topLeftY;

	//current cursor position
	double currentXPos;
	double currentYPos;

	glm::mat4 projection;

	bool active;
	bool inputMode;
	bool shouldDrawTemp;

	float size;

	std::map<char, Character> characters;

	std::vector<int> charBuffer;

	void handleMouseClick(int button, int action, double xPosition, double yPosition) override;
	void handleMouseMovement(double xPosition, double yPosition, double xPrevPosition, double yPrevPosition) override;
	void handleKeyboardInput(int key, int action) override;
};
