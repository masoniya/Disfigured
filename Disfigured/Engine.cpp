#include <iostream>

#include <glad/glad.h>

#include "Engine.h"
#include "InputManager.h"
#include "save.h"
#include "ColorTransform.h"


int WIDTH = 800;
int HEIGHT = 600;

const char* imageVertPath = "shaders/imageShader.vert";
const char* imageFragPath = "shaders/imageShader.frag";

const char* brushVertPath = "shaders/brushShader.vert";
const char* brushFragPath = "shaders/brushShader.frag";

const char* pencilGeomPath = "shaders/pencilShader.geom";
const char* eraserGeomPath = "shaders/eraserShader.geom";

const char* caligFGeomPath = "shaders/caligFShader.geom";
const char* caligBGeomPath = "shaders/caligBShader.geom";

const char* airbrushGeomPath = "shaders/airbrushShader.geom";
const char* airbrushFragPath = "shaders/airbrushShader.frag";

const char* markerGeomPath = "shaders/markerShader.geom";
const char* markerFragpath = "shaders/markerShader.frag";

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
	lineProgram = new ShaderProgram(brushVertPath, brushFragPath);
	pencilProgram = new ShaderProgram(brushVertPath, pencilGeomPath, brushFragPath); //pencil
	eraserProgram = new ShaderProgram(brushVertPath, eraserGeomPath, brushFragPath); //eraser
	caligFProgram = new ShaderProgram(brushVertPath, caligFGeomPath, brushFragPath); //caligraphy brush (forward)
	caligBProgram = new ShaderProgram(brushVertPath, caligBGeomPath, brushFragPath); //caligraphy brush (backward)
	markerProgram = new ShaderProgram(brushVertPath, markerGeomPath, markerFragpath); //marker
	airbrushProgram = new ShaderProgram(brushVertPath, airbrushGeomPath, airbrushFragPath); //airbrush

	image = loadImage("resources/coo.png");

	//pencil
	pencil = new Brush(pencilProgram, 1.0f);
	registerBrush(pencil);

	//eraser
	eraser = new Brush(eraserProgram, ColorType::background);
	registerBrush(eraser);

	//caligraphy brush (forward)
	caligFBrush = new Brush(caligFProgram, 15.0f);
	registerBrush(caligFBrush);

	//caligraphy brush (backward)
	caligBBrush = new Brush(caligBProgram, 15.0f);
	registerBrush(caligBBrush);

	//airbrush
	airbrush = new AirBrush(airbrushProgram, 40.0f);
	registerBrush(airbrush);

	//marker
	marker = new Marker(markerProgram, 25.0f);
	registerBrush(marker);

	//active pencil by default
	activeBrush = pencil;
	activeBrush->use();

	//create color box then switch back to main window
	colorBox = new ColorBox();
	window->setActive();

	//filltool
	fillTool = new FillTool();
	InputManager::registerMouseClickInput(fillTool);
	fillTool->use();

	//color picker
	colorPicker = new ColorPicker();
	InputManager::registerMouseClickInput(colorPicker);

	//image vertex data
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

	//choose front buffer as both draw and read target
	glDrawBuffer(GL_FRONT);
	glReadBuffer(GL_FRONT);

	glClear(GL_COLOR_BUFFER_BIT);

	//no brush uses this for now
	glLineWidth(4.0f);

	//enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Engine::mainLoop()
{
	while (!window->shouldClose()) {

		renderFrame();

		//glfwPollEvents(); //for uncapped framerate
		//glfwWaitEventsTimeout(0.01677777); //around 60 fps
		glfwWaitEventsTimeout(0.03444444); //around 30 fps
		//glfwWaitEvents(); //for complete input waiting
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

void Engine::saveImageToFile(std::string path)
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

void Engine::saveFrameToImage()
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

	if (activeBrush == airbrush) {
		airbrush->update();
	}

	if (activeBrush->shouldDrawLines()) {
		activeBrush->draw();

		glFlush();
		frameSaved = false;
	}

	if (fillTool->shouldDrawFrame()) {
		fillTool->drawFrame(imageProgram, vao);

		glFlush();
		frameSaved = false;
	}

	if (!frameSaved) {
		saveFrameToImage();
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
	//Tool controls
	if (key == GLFW_KEY_S && action == GLFW_PRESS) { // s to save
		std::cout << "Saved image" << std::endl;
		saveImageToFile("output/niqqa.png");
	}
	else if (key == GLFW_KEY_P && action == GLFW_PRESS) { // hold p for color picker
		std::cout << "Color picker active" << std::endl;
		colorPicker->use();
		activeBrush->unuse();

	}
	else if (key == GLFW_KEY_P && action == GLFW_RELEASE) { // release p for no color picker
		std::cout << "Color picker inactive" << std::endl;
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
	else if (key == GLFW_KEY_5 && action == GLFW_PRESS) { // 5 for airbrush
		useBrush(airbrush);
	}
	else if (key == GLFW_KEY_6 && action == GLFW_PRESS) { // 6 for marker
		useBrush(marker);
	}
	
	//Console input controls
	else if (key == GLFW_KEY_C && action == GLFW_PRESS) { // c to change color
		int colorChoice;
		std::cout << "Change foreground color (1)" << std::endl << "Change background color (2)" << std::endl << "Cancel (3)" << std::endl;
		std::cin >> colorChoice;
		
		if (colorChoice == 3) {
			return;
		}

		if (colorChoice == 1 || colorChoice == 2) {
			int modeChoice;
			std::cout << "RGB (1)" << std::endl << "HSL (2)" << std::endl << "Cancel (3)" << std::endl;
			std::cin >> modeChoice;

			if (modeChoice == 3) {
				return;
			}

			if (modeChoice == 1) {
				float r, g, b;
				std::cout << "Enter new RGB Color (0 - 255) : " << std::endl;
				std::cout << "R - G - B" << std::endl;
				std::cin >> r >> g >> b;

				r = glm::clamp(r, 0.0f, 255.0f);
				g = glm::clamp(g, 0.0f, 255.0f);
				b = glm::clamp(b, 0.0f, 255.0f);

				r /= 255.0f;
				g /= 255.0f;
				b /= 255.0f;

				switch (colorChoice) {
				case 1:
					ColorBox::foregroundColor = glm::vec3(r, g, b);
				case 2:
					ColorBox::backgroundColor = glm::vec3(r, g, b);
				}
			}

			else if (modeChoice == 2) {
				float h, s, l, r, g, b;
				std::cout << "Enter new HSL Color (0 - 240) : " << std::endl;
				std::cout << "H - S - L" << std::endl;
				std::cin >> h >> s >> l;

				h = glm::clamp(h, 0.0f, 240.0f);
				s = glm::clamp(s, 0.0f, 240.0f);
				l = glm::clamp(l, 0.0f, 240.0f);

				h /= 240.0f;
				s /= 240.0f;
				l /= 240.0f;

				hsl_to_rgb(h, s, l, &r, &g, &b);

				switch (colorChoice) {
				case 1:
					ColorBox::foregroundColor = glm::vec3(r, g, b);
				case 2:
					ColorBox::backgroundColor = glm::vec3(r, g, b);
				}
			}

		}

		system("cls");
	}

}
