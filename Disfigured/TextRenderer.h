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


struct Character {
	GLuint texture;
	glm::ivec2 size;
	glm::ivec2 bearing;
	GLuint advance;
};


class TextRenderer : public Tool, public MouseClickControl, public KeyboardControl
{
public:
	TextRenderer(std::string fontPath);

	void renderText(ShaderProgram * program);

	void use() override;
	void unuse() override;

	bool isActive();
	bool isInputMode();

	void increaseSize();
	void decreaseSize();

	bool shouldDrawText();

	void recalculateProjection();

	void handleMouseClick(int button, int action, double xPosition, double yPosition) override;
	void handleKeyboardInput(int key, int action) override;

private:

	unsigned int vao;
	unsigned int vbo;

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

	glm::mat4 projection;

	bool active;
	bool inputMode;

	float size;

	std::map<char, Character> characters;

	std::vector<int> charBuffer;
};
