#pragma once

#include <vector>

#include "ShaderProgram.h"


class Canvas
{
public:
	Canvas();

	void copyToScreen(ShaderProgram * program);

	void use();
	void unuse();

	void resize();

	void changeVisibleArea(std::vector<float> vertices);

private:

	unsigned int fbo;
	unsigned int rbo;
	unsigned int texture;

	int width;
	int height;

	unsigned int vao;
	unsigned int vbo;
};
