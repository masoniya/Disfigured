#pragma once

#include "ShaderProgram.h"


class Canvas
{
public:
	Canvas();

	void copyToScreen(ShaderProgram * program, unsigned int vao);

	void use();
	void unuse();

	void resize();

private:
	unsigned int fbo;
	//unsigned int rbo;
	unsigned int texture;
	int width;
	int height;
};
