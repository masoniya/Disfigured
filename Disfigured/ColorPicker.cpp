#include <glad/glad.h>

#include "ColorPicker.h"
#include "CoordinateTransform.h"
#include "Window.h"
#include "ColorBox.h"


ColorPicker::ColorPicker() :
	active(false)
{
}


void ColorPicker::use()
{
	active = true;
}

void ColorPicker::unuse()
{
	active = false;
}

void ColorPicker::handleMouseClick(int button, int action, double xPosition, double yPosition)
{
	if (active) {
		if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
			unsigned char* pixel = new unsigned char[4];
			double xPos = 0, yPos = 0;
			normalizedScreenToScreenCoords(xPosition, yPosition, &xPos, &yPos, Window::width, Window::height);
			glReadPixels((int)xPos, Window::height - (int)yPos, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
			ColorBox::foregroundColor = glm::vec3(pixel[0] / 255.0f, pixel[1] / 255.0f, pixel[2] / 255.0f);
		}
	}
}
