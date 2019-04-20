#pragma once

#include <glfw/glfw3.h>
#include <glm/glm.hpp>


class ColorBox
{
public:
	ColorBox();

	void setActive();

	static int width;
	static int height;

	static double mouseXPos;
	static double mouseYPos;

	static glm::vec3 foregroundColor;
	static glm::vec3 backgroundColor;
private:
	GLFWwindow* window;
};
