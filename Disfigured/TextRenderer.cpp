#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "TextRenderer.h"


TextRenderer::TextRenderer(std::string fontPath)
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		std::cout << "Error... Could not init FreeType Library" << std::endl;
	}

	FT_Face face;
	if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
		std::cout << "Error... Failed to load font : " << fontPath << std::endl;
	}

	std::cout << "initialized text renderer 1" << std::endl;

	FT_Set_Pixel_Sizes(face, 0, 50);

	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER)) {
		std::cout << "Error... Failed to load glyph" << std::endl;
	}

	std::cout << "initialized text renderer 2" << std::endl;
}
