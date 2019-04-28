#pragma once

#include <string>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

	unsigned int vao;
	unsigned int vbo;

	std::map<char, Character> characters;

	glm::mat4 projection;
};
