#pragma once

#include <string>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H


struct Character {
	GLuint texture;
	glm::ivec2 size;
	glm::ivec2 bearing;
	GLuint advance;
};


class TextRenderer
{
public:
	TextRenderer(std::string fontPath);

private:
	std::map<char, Character> characters;
};
