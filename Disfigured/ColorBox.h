#pragma once

#include <glfw/glfw3.h>
#include <glm/glm.hpp>


//Separate window to get colors from by clicking
class ColorBox
{
public:
	ColorBox();

	void setActive();

	static int width;
	static int height;

	static double mouseXPos;
	static double mouseYPos;

	//Actual colors of brushes and eraser
	static glm::vec3 foregroundColor;
	static glm::vec3 backgroundColor;

private:

	GLFWwindow* window;
};
