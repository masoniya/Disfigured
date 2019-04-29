#include <iostream>

#include <glad/glad.h>

#include "TextRenderer.h"
#include "Window.h"
#include "ColorBox.h"
#include "CoordTransform.h"


TextRenderer::TextRenderer(std::string fontPath) :
	active(false),
	inputMode(false),
	size(1.0f),
	characterHeight(24),
	lineSpacing(3)
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		std::cout << "Error... Could not init FreeType Library" << std::endl;
	}

	FT_Face face;
	if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
		std::cout << "Error... Failed to load font : " << fontPath << std::endl;
	}

	//0 for width causes the loader to calculate the width based on the height
	FT_Set_Pixel_Sizes(face, 0, characterHeight);

	//Load all ascii characters up to 128
	for (GLubyte c = 0; c < 128; c++) {
		//Load character
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "Failed to load glyph : " << c << std::endl;
			continue;
		}
		
		//generate texture for character
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		//texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		//store the character
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(GLuint)face->glyph->advance.x
		};
		characters.emplace(c, character);
	}

	//clear resources after loading the glyphs
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	projection = glm::ortho(0.0f, (float)Window::width, 0.0f, (float)Window::height);

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

//Draw all clicked keys in input mode
void TextRenderer::renderText(ShaderProgram * program)
{
	//set shader uniforms and bind vao
	program->use();
	program->setUniformMat4("projection", projection);
	program->setUniformVec3("textColor", ColorBox::foregroundColor);
	program->setUniformInt("textImage", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(vao);

	for (int c : charBuffer) {
		if (c == 257) { //new line
			topLeftX = (float)fmin(x, x2);
			topLeftY -= characterHeight * size + lineSpacing;
			continue;
		}
		
		Character ch = characters[(char)c];

		float xPos = (float)(topLeftX + ch.bearing.x * size);
		float yPos = (float)(topLeftY - (ch.size.y - ch.bearing.y) * size);

		float w = ch.size.x * size;
		float h = ch.size.y * size;

		float vertices[6][4] = {
			{xPos,		yPos + h,	0.0f, 0.0f},
			{xPos,		yPos,		0.0f, 1.0f},
			{xPos + w,	yPos,		1.0f, 1.0f},
			{xPos,		yPos + h,	0.0f, 0.0f},
			{xPos + w,	yPos,		1.0f, 1.0f},
			{xPos + w,	yPos + h,	1.0f, 0.0f}
		};

		//render glyph texture
		glBindTexture(GL_TEXTURE_2D, ch.texture);

		//update vbo contents
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		topLeftX += (ch.advance >> 6) * size;
		//some way to next line or sth
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	charBuffer.clear();
}

void TextRenderer::use()
{
	active = true;
}

void TextRenderer::unuse()
{
	active = false;
}

bool TextRenderer::isActive()
{
	return active;
}

bool TextRenderer::isInputMode()
{
	return inputMode;
}

void TextRenderer::increaseSize()
{
	size += 0.1f;
}

void TextRenderer::decreaseSize()
{
	size -= 0.1f;
	if (size < 0.0f) {
		size = 0.0f;
	}
}

bool TextRenderer::shouldDrawText()
{
	return !charBuffer.empty();
}

void TextRenderer::recalculateProjection()
{
	projection = glm::ortho(0.0f, (float)Window::width, 0.0f, (float)Window::height);
}

void TextRenderer::handleMouseClick(int button, int action, double xPosition, double yPosition)
{
	if (active) {
		if (inputMode) {
			if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE) {
				inputMode = false;
				glDisable(GL_SCISSOR_TEST);
				std::cout << "Select mode" << std::endl;
			}
		}
		else {
			if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
				normalizedScreenToFlippedScreenCoords(xPosition, yPosition, &x, &y, Window::width, Window::height);
			}
			if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE) {
				normalizedScreenToFlippedScreenCoords(xPosition, yPosition, &x2, &y2, Window::width, Window::height);

				int width = abs((int)(x2 - x));
				int height = abs((int)(y2 - y));

				float bottomLeftX = (float)fmin(x, x2);
				float bottomLeftY = (float)fmin(y, y2);
				
				//bottomLeftY -= characterHeight * size + lineSpacing;

				topLeftX = (float)fmin(x, x2);
				topLeftY = (float)fmax(y, y2);

				topLeftY -= characterHeight * size + lineSpacing;

				glScissor((int)bottomLeftX, (int)bottomLeftY, width, height);

				inputMode = true;
				glEnable(GL_SCISSOR_TEST);
				std::cout << "Input mode" << std::endl;
			}
		}
	}
}

void TextRenderer::handleKeyboardInput(int key, int action)
{
	if (active) {
		if (inputMode) {
			if (action == GLFW_PRESS) {
				charBuffer.push_back(key);
			}
		}
	}
}
