#include <iostream>

#include <glad/glad.h>

#include "Engine.h"
#include "InputManager.h"
#include "save.h"


int WIDTH = 800;
int HEIGHT = 600;

const char* imageVertPath = "shaders/imageShader.vert";
const char* imageFragPath = "shaders/imageShader.frag";

const char* lineVertPath = "shaders/lineShader.vert";
const char* lineFragPath = "shaders/lineShader.frag";

const char* squareGeomPath = "shaders/squareShader.geom";
const char* circleGeomPath = "shaders/circleShader.geom";

const float imageVertices[] = {
	-1.0, -1.0,  0.0, 0.0,
	-1.0,  1.0,  0.0, 1.0,
	 1.0, -1.0,  1.0, 0.0,
	 1.0,  1.0,  1.0, 1.0,
	 1.0, -1.0,  1.0, 0.0,
	-1.0,  1.0,  0.0, 1.0,
};

void Engine::start()
{
	init();
	mainLoop();
	cleanup();
}

void Engine::init()
{
	InputManager::registerKeyboardInput(this);
	window = new Window(WIDTH, HEIGHT, "Plastic Surgery");
	imageProgram = new ShaderProgram(imageVertPath, imageFragPath);
	lineProgram = new ShaderProgram(lineVertPath, lineFragPath);
	pencilProgram = new ShaderProgram(lineVertPath, circleGeomPath, lineFragPath); //pencil
	eraserProgram = new ShaderProgram(lineVertPath, squareGeomPath, lineFragPath); //eraser
	image = loadImage("resources/coo.png");

	pencil = new Pencil();
	InputManager::registerMouseClickInput(pencil);
	InputManager::registerMouseMoveInput(pencil);
	pencil->use();

	eraser = new Eraser();
	InputManager::registerMouseClickInput(eraser);
	InputManager::registerMouseMoveInput(eraser);
	eraser->use();

	colorBox = new ColorBox();
	window->setActive();

	fillTool = new FillTool();
	InputManager::registerMouseClickInput(fillTool);
	fillTool->use();

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(imageVertices), imageVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindVertexArray(0);

	glDrawBuffer(GL_FRONT);
	glReadBuffer(GL_FRONT);
	glClear(GL_COLOR_BUFFER_BIT);

	glLineWidth(4.0f);
}

void Engine::mainLoop()
{
	while (!window->shouldClose()) {

		renderFrame();

		//glfwPollEvents();
		glfwWaitEvents();
	}
}

void Engine::cleanup()
{
	delete imageProgram;
	window->close();
	delete window;
}

Texture * Engine::loadImage(std::string path)
{
	shouldDrawImage = true;
	return new Texture(path);
}

void Engine::saveImage(std::string path)
{
	unsigned char* pixels = new unsigned char[Window::width * Window::height * 4];
	glReadPixels(0, 0, Window::width, Window::height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	save_image(path.c_str(), Window::width, Window::height, 4, pixels, Window::width * 4);
}

void Engine::drawImage(Texture * image)
{
	glActiveTexture(GL_TEXTURE0);
	image->use();

	imageProgram->use();
	imageProgram->setUniformInt("image", 0);

	glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Engine::renderFrame()
{
	if (shouldDrawImage) {
		//window->swapBuffers();

		drawImage(image);
		shouldDrawImage = false;

		glFlush();
		//window->swapBuffers();
	}

	if (pencil->shouldDrawLines()) {
		pencil->drawLines(pencilProgram);
		glFlush();
	}

	if (eraser->shouldDrawLines()) {
		eraser->drawLines(eraserProgram);
		glFlush();
	}

	if (fillTool->shouldDrawFrame()) {
		fillTool->drawFrame(imageProgram, vao);
		glFlush();
	}

	//glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

//master keyboard control
void Engine::handleKeyboardInput(int key, int action)
{
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		saveImage("output/niqqa.png");
	}
	else if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		//something to switch to color picker
	}
}
