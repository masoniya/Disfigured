#include <iostream>

#include "InputManager.h"
#include "CoordTransform.h"
#include "Window.h"


static void keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
static void mouse_click_callback(GLFWwindow *window, int button, int action, int mods);
static void mouse_move_callback(GLFWwindow *window, double xPosition, double yPosition);
static void mouse_scroll_callback(GLFWwindow *window, double xOffset, double yOffset);
static void file_drop_callback(GLFWwindow *window, int count, const char** paths);


GLFWwindow* InputManager::window;

std::vector<KeyboardControl*> InputManager::keyboardControlObjects;
std::vector<MouseClickControl*> InputManager::mouseClickControlObjects;
std::vector<MouseMoveControl*> InputManager::mouseMoveControlObjects;
std::vector<MouseScrollControl*> InputManager::mouseScrollControlObjects;
std::vector<FileDropControl*> InputManager::fileDropControlObjects;

double InputManager::cursorLastXPos;
double InputManager::cursorLastYPos;

void InputManager::registerWindow(GLFWwindow * window)
{
	InputManager::window = window;

	glfwSetKeyCallback(window, keyboard_callback);
	glfwSetMouseButtonCallback(window, mouse_click_callback);
	glfwSetCursorPosCallback(window, mouse_move_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);
	glfwSetDropCallback(window, file_drop_callback);
}

void InputManager::registerKeyboardInput(KeyboardControl* object)
{
	keyboardControlObjects.push_back(object);
}

void InputManager::registerMouseClickInput(MouseClickControl* object)
{
	mouseClickControlObjects.push_back(object);
}

void InputManager::registerMouseMoveInput(MouseMoveControl* object)
{
	mouseMoveControlObjects.push_back(object);
}

void InputManager::registerMouseScrollInput(MouseScrollControl* object)
{
	mouseScrollControlObjects.push_back(object);
}

void InputManager::registerFileDropInput(FileDropControl * object)
{
	fileDropControlObjects.push_back(object);
}

void InputManager::handleKeyboardInput(int key, int action)
{
	//std::cout << "key : " << key << " action : " << action << std::endl;
	for (KeyboardControl* object : keyboardControlObjects) {
		object->handleKeyboardInput(key, action);
	}
}

void InputManager::handleMouseClickInput(int button, int action)
{
	//std::cout << "button : " << button << " action : " << action << std::endl;
	for (MouseClickControl* object : mouseClickControlObjects) {
		object->handleMouseClick(button, action, cursorLastXPos, cursorLastYPos);
	}
}

void InputManager::handleMouseMoveInput(double xPosition, double yPosition)
{
	double normalizedXPosition;
	double normalizedYPosition;
	screenToNormalizedScreenCoords(xPosition, yPosition, &normalizedXPosition, &normalizedYPosition, Window::width, Window::height);

	static bool firstMove = true;
	if (firstMove) {
		firstMove = false;
		cursorLastXPos = normalizedXPosition;
		cursorLastYPos = normalizedYPosition;
	}

	//std::cout << "xPosition : " << normalizedXPosition << " yPosition : " << normalizedYPosition << std::endl;
	for (MouseMoveControl* object : mouseMoveControlObjects) {
		object->handleMouseMovement(normalizedXPosition, normalizedYPosition, cursorLastXPos, cursorLastYPos);
	}
	cursorLastXPos = normalizedXPosition;
	cursorLastYPos = normalizedYPosition;
}

void InputManager::handleMouseScrollInput(double xOffset, double yOffset)
{
	//std::cout << "xOffset : " << xOffset << " yOffset : " << yOffset << std::endl;
	for (MouseScrollControl* object : mouseScrollControlObjects) {
		object->handleMouseScroll(xOffset, yOffset);
	}
}

void InputManager::handleFileDrop(const char* path)
{
	for (FileDropControl* object : fileDropControlObjects) {
		object->handleFileDrop(path);
	}
}

bool InputManager::controlDown()
{
	return glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;
}

bool InputManager::shiftDown()
{
	return glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
}

bool InputManager::altDown()
{
	return glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS;
}


void keyboard_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	InputManager::handleKeyboardInput(key, action);
}

void mouse_click_callback(GLFWwindow * window, int button, int action, int mods)
{
	InputManager::handleMouseClickInput(button, action);
}

void mouse_move_callback(GLFWwindow * window, double xPosition, double yPosition)
{
	InputManager::handleMouseMoveInput(xPosition, yPosition);
}

void mouse_scroll_callback(GLFWwindow * window, double xOffset, double yOffset)
{
	InputManager::handleMouseScrollInput(xOffset, yOffset);
}

static void file_drop_callback(GLFWwindow *window, int count, const char** paths) 
{
	InputManager::handleFileDrop(paths[0]);
}
