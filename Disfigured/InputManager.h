#pragma once

#include <unordered_map>
#include <vector>

#include <glfw/glfw3.h>

#include "KeyboardControl.h"
#include "MouseClickControl.h"
#include "MouseMoveControl.h"
#include "MouseScrollControl.h"


class InputManager
{
public:
	static void registerWindow(GLFWwindow* window);

	static void registerKeyboardInput(KeyboardControl* object);
	static void registerMouseClickInput(MouseClickControl* object);
	static void registerMouseMoveInput(MouseMoveControl* object);
	static void registerMouseScrollInput(MouseScrollControl* object);

	//static void updateAllInputObjects();

	static void handleKeyboardInput(int key, int action);
	static void handleMouseClickInput(int button, int action);
	static void handleMouseMoveInput(double xPosition, double yPosition);
	static void handleMouseScrollInput(double xOffset, double yOffset);


private:
	static GLFWwindow *window;

	//static std::unordered_map<int, int> buttonMap;

	static std::vector<KeyboardControl*> keyboardControlObjects;
	static std::vector<MouseClickControl*> mouseClickControlObjects;
	static std::vector<MouseMoveControl*> mouseMoveControlObjects;
	static std::vector<MouseScrollControl*> mouseScrollControlObjects;

	static double cursorLastXPos;
	static double cursorLastYPos;
};
