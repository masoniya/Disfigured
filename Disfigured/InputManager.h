#pragma once

#include <unordered_map>
#include <vector>

#include <glfw/glfw3.h>

#include "KeyboardControl.h"
#include "MouseClickControl.h"
#include "MouseMoveControl.h"
#include "MouseScrollControl.h"
#include "FileDropControl.h"


//Master input control class
//Provides interfaces for getting keyboard, mouse, and file drop controls
//Objects that require input should implement the necessary interface and register in the input manager
//The input manager automaticaly sends proper input data to each registered object on input
//Provides the ability to swap window to track input for
//Also provides easy tracking of keyboard modifiers

class InputManager
{
public:
	//select window to track input for
	static void registerWindow(GLFWwindow* window);

	//functions to register input objeccts
	static void registerKeyboardInput(KeyboardControl* object);
	static void registerMouseClickInput(MouseClickControl* object);
	static void registerMouseMoveInput(MouseMoveControl* object);
	static void registerMouseScrollInput(MouseScrollControl* object);
	static void registerFileDropInput(FileDropControl* object);

	//functions to deliver input data to objects
	static void handleKeyboardInput(int key, int action);
	static void handleMouseClickInput(int button, int action);
	static void handleMouseMoveInput(double xPosition, double yPosition);
	static void handleMouseScrollInput(double xOffset, double yOffset);
	static void handleFileDrop(const char* path);

	//helper functions for keyboard modifiers
	static bool controlDown();
	static bool shiftDown();
	static bool altDown();

private:
	
	//the window to track input for
	static GLFWwindow *window;

	//containers for the input controlled objects
	static std::vector<KeyboardControl*> keyboardControlObjects;
	static std::vector<MouseClickControl*> mouseClickControlObjects;
	static std::vector<MouseMoveControl*> mouseMoveControlObjects;
	static std::vector<MouseScrollControl*> mouseScrollControlObjects;
	static std::vector<FileDropControl*> fileDropControlObjects;

	//useful for mouse movement and mouse click controled objects
	static double cursorLastXPos;
	static double cursorLastYPos;
};
