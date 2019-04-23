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

const char* fSlashGeomPath = "shaders/fSlashShader.geom";
const char* bSlashGeomPath = "shaders/bSlashShader.geom";

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

	imageLoaded = false;
	frameSaved = true;

	window = new Window(WIDTH, HEIGHT, "Plastic Surgery");
	imageProgram = new ShaderProgram(imageVertPath, imageFragPath);
	lineProgram = new ShaderProgram(lineVertPath, lineFragPath);
	pencilProgram = new ShaderProgram(lineVertPath, circleGeomPath, lineFragPath); //pencil
	eraserProgram = new ShaderProgram(lineVertPath, squareGeomPath, lineFragPath); //eraser
	caligFProgram = new ShaderProgram(lineVertPath, fSlashGeomPath, lineFragPath); //caligraphy brush (forward)
	caligBProgram = new ShaderProgram(lineVertPath, bSlashGeomPath, lineFragPath); //caligraphy brush (backward)

	image = loadImage("resources/coo.png");

	pencil = new Brush(pencilProgram, 1.0f);
	registerBrush(pencil);

	eraser = new Brush(eraserProgram, ColorType::background);
	registerBrush(eraser);

	caligFBrush = new Brush(caligFProgram, 15.0f);
	registerBrush(caligFBrush);

	caligBBrush = new Brush(caligBProgram, 15.0f);
	registerBrush(caligBBrush);

	activeBrush = pencil;
	activeBrush->use();

	colorBox = new ColorBox();
	window->setActive();

	fillTool = new FillTool();
	InputManager::registerMouseClickInput(fillTool);
	fillTool->use();

	colorPicker = new ColorPicker();
	InputManager::registerMouseClickInput(colorPicker);
	//colorPicker->use(); //don't activate it yet

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

void Engine::registerBrush(Brush * brush)
{
	InputManager::registerMouseClickInput(brush);
	InputManager::registerMouseMoveInput(brush);
}

void Engine::useBrush(Brush * brush)
{
	activeBrush->unuse();
	brush->use();
	activeBrush = brush;
}

Texture * Engine::loadImage(std::string path)
{
	imageLoaded = true;
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
	glClear(GL_COLOR_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	image->use();

	imageProgram->use();
	imageProgram->setUniformInt("image", 0);

	glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Engine::saveFrame()
{
	unsigned char * pixels = new unsigned char[Window::width * Window::height * 4];
	glReadPixels(0, 0, Window::width, Window::height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	image->updateContents(Window::width, Window::height, pixels);

	delete pixels;

	frameSaved = true;
}

void Engine::renderFrame()
{
	if (imageLoaded) { //draw a freshly loaded image on an empty canvas
		drawImage(image);
		imageLoaded = false;

		glFlush();
		frameSaved = false;
	}

	if (activeBrush->shouldDrawLines()) {
		activeBrush->drawLines();

		glFlush();
		frameSaved = false;
	}

	if (fillTool->shouldDrawFrame()) {
		fillTool->drawFrame(imageProgram, vao);

		glFlush();
		frameSaved = false;
	}

	if (!frameSaved) {
		saveFrame();
	}

	if (Window::windowResized) {
		drawImage(image);
		glFlush();
	}

	glFlush();
}

//master keyboard control
void Engine::handleKeyboardInput(int key, int action)
{
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		saveImage("output/niqqa.png");
	}
	else if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		colorPicker->use();
		activeBrush->unuse();

	}
	else if (key == GLFW_KEY_P && action == GLFW_RELEASE) {
		colorPicker->unuse();
		activeBrush->use();
	}
	
	//Brush controls
	else if (key == GLFW_KEY_1 && action == GLFW_PRESS) { // 1 for pencil
		useBrush(pencil);
	}
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS) { // 2 for eraser
		useBrush(eraser);
	}
	else if (key == GLFW_KEY_3 && action == GLFW_PRESS) { // 3 for caligraphy brush (forward)
		useBrush(caligFBrush);
	}
	else if (key == GLFW_KEY_4 && action == GLFW_PRESS) { // 4 for caligraphy brush (backward)
		useBrush(caligBBrush);
	}
	
}
