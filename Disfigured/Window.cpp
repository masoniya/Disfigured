#include <iostream>

#include <glad/glad.h>

#include "Window.h"
#include "InputManager.h"
#include "Engine.h"


static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
static void windowFocusCallback(GLFWwindow* window, int focused);


int Window::width;
int Window::height;
bool Window::windowResized = false;

Window::Window(int width, int height, const char * name)
{
	init(width, height, name);
}

void Window::init(int width, int height, const char * name)
{
	//only initialize the window once
	if (window != nullptr) {
		return;
	}

	Window::width = width;
	Window::height = height;

	//create window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, name, nullptr, nullptr);

	if (window == nullptr) {
		throw std::runtime_error("Failed to create GLFW window");
	}

	glfwSetWindowPos(window, 600, 100);

	//set opengl context
	glfwMakeContextCurrent(window);

	//load opengl functions (only required once per program)
	static bool gladLoaded = false;
	if (!gladLoaded) {
		gladLoaded = true;
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			throw std::runtime_error("Failed to initialize GLAD.");
		}
	}

	//set framebuffer size
	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetWindowFocusCallback(window, windowFocusCallback);

	InputManager::registerWindow(window);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void Window::swapBuffers()
{
	glfwSwapBuffers(window);
}

void Window::setClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

bool Window::shouldClose()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	return glfwWindowShouldClose(window);
}

void Window::close()
{
	glfwTerminate();
}

void Window::checkActive()
{
	if (glfwGetWindowAttrib(window, GLFW_FOCUSED)){
		;
	}
}

void Window::setActive()
{
	glfwFocusWindow(window);
}

static void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
	Window::width = width;
	Window::height = height;
	Window::windowResized = true;

}

static void windowFocusCallback(GLFWwindow* window, int focused)
{
	if (focused)
	{
		glfwMakeContextCurrent(window);
		glViewport(0, 0, Window::width, Window::height);
	}
	else
	{
		;
	}
}
