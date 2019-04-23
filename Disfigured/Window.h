#pragma once

#include <stdexcept>

#include <glfw/glfw3.h>

#include "ShaderProgram.h"


class Window
{
public:
	Window(int width, int height, const char* name);
	
	void swapBuffers();
	void setClearColor(float r, float g, float b, float a);
	bool shouldClose();
	void close();
	void checkActive();
	void setActive();

	static int width;
	static int height;
	static bool windowResized;

private:

	void init(int width, int height, const char* name);

	GLFWwindow *window;
};
