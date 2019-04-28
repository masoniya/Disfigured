#include <iostream>

#include <glad/glad.h>

#include "Engine.h"
#include "InputManager.h"
#include "save.h"
#include "ColorTransform.h"


int WIDTH = 800;
int HEIGHT = 600;

const char* imageVertPath = "resources/shaders/imageShader.vert";
const char* imageFragPath = "resources/shaders/imageShader.frag"; 

const char* brushVertPath = "resources/shaders/brushShader.vert";
const char* brushFragPath = "resources/shaders/brushShader.frag";

const char* pencilGeomPath = "resources/shaders/pencilShader.geom";
const char* eraserGeomPath = "resources/shaders/eraserShader.geom";

const char* caligFGeomPath = "resources/shaders/caligFShader.geom";
const char* caligBGeomPath = "resources/shaders/caligBShader.geom";

const char* airbrushGeomPath = "resources/shaders/airbrushShader.geom";
const char* airbrushFragPath = "resources/shaders/airbrushShader.frag";

const char* markerGeomPath = "resources/shaders/markerShader.geom";
const char* markerFragpath = "resources/shaders/markerShader.frag";

std::string fontPath = "resources/fonts/arial.ttf";

const float imageVertices[] = {
	-1.0, -1.0,  0.0, 0.0, //bottom left
	-1.0,  1.0,  0.0, 1.0, //top left
	 1.0, -1.0,  1.0, 0.0, //bottom right
	 1.0,  1.0,  1.0, 1.0, //top right
	 1.0, -1.0,  1.0, 0.0, //bottom right
	-1.0,  1.0,  0.0, 1.0, //top left
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
	InputManager::registerFileDropInput(this);

	imageLoaded = false;
	frameSaved = true;
	saveDirectory = "output/";
	saveName = "image.png";

	window = new Window(WIDTH, HEIGHT, "Plastic Surgery");

	canvas = new Canvas();
	canvas->use();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //reading from client memory
	glPixelStorei(GL_PACK_ALIGNMENT, 1); //writing to client memory

	initShaders();
	
	initBrushes();
	
	initTools();

	initVertexData();

	//choose front buffer as both draw and read target
	glDrawBuffer(GL_FRONT);
	glReadBuffer(GL_FRONT);
	
	//prepare buffer for drawing
	glClear(GL_COLOR_BUFFER_BIT);

	//glEnable(GL_LINE_SMOOTH);
	//glLineWidth(4.0f);

	//enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	image = loadImage("resources/textures/coo.png"); // don't load images before setting pack/unpack alignment

	//glEnable(GL_SCISSOR_TEST);
}

void Engine::initShaders()
{
	imageProgram = new ShaderProgram(imageVertPath, imageFragPath); //frame image
	lineProgram = new ShaderProgram(brushVertPath, brushFragPath); //lines (unused)
	pencilProgram = new ShaderProgram(brushVertPath, pencilGeomPath, brushFragPath); //pencil
	eraserProgram = new ShaderProgram(brushVertPath, eraserGeomPath, brushFragPath); //eraser
	caligFProgram = new ShaderProgram(brushVertPath, caligFGeomPath, brushFragPath); //caligraphy brush (forward)
	caligBProgram = new ShaderProgram(brushVertPath, caligBGeomPath, brushFragPath); //caligraphy brush (backward)
	markerProgram = new ShaderProgram(brushVertPath, markerGeomPath, markerFragpath); //marker
	airbrushProgram = new ShaderProgram(brushVertPath, airbrushGeomPath, airbrushFragPath); //airbrush
}

void Engine::initBrushes()
{
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

	//activate pencil by default
	activeBrush = pencil;
	activeBrush->use();
}

void Engine::initTools()
{
	//create color box then switch back to main window
	colorBox = new ColorBox();
	window->setActive();

	//filltool
	fillTool = new FillTool();
	InputManager::registerMouseClickInput(fillTool);

	//color picker
	colorPicker = new ColorPicker();
	InputManager::registerMouseClickInput(colorPicker);

	//clipboard
	clipBoard = new ClipBoard();
	InputManager::registerMouseClickInput(clipBoard);

	//text renderer
	textRenderer = new TextRenderer(fontPath);
	//input for the text renderer
	
	//activate color picker by default
	activeTool = colorPicker;
}

void Engine::initVertexData()
{
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
	activeTool->unuse();
	activeBrush->unuse();

	activeBrush = brush;
	activeBrush->use();
}

void Engine::useTool(Tool * tool)
{
	activeBrush->unuse();
	activeTool->unuse();

	activeTool = tool;
	activeTool->use();
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
	std::cout << "drawing image" << std::endl;

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
	//could be using glCopyTexImage2D()

	unsigned char * pixels = new unsigned char[Window::width * Window::height * 4];
	glReadPixels(0, 0, Window::width, Window::height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	image->updateContents(Window::width, Window::height, pixels, 3);

	delete pixels;

	frameSaved = true;
}

void Engine::reloadImage(std::string path)
{
	image->reload(path);
	drawImage(image);
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

	if (clipBoard->shouldDrawImage()) {
		clipBoard->drawImage(imageProgram);

		glFlush();
		frameSaved = false;
	}

	//always check if the frame changed
	if (!frameSaved) {
		saveFrameToImage();
		frameChanged = true;
	}

	//check window resize last
	if (Window::windowResized) {
		canvas->resize();
		drawImage(image);
		Window::windowResized = false;
		frameChanged = true;
		
		glFlush();
	}
	
	if (frameChanged) {
		//copy the canvas contents to the screen
		canvas->unuse();
		canvas->copyToScreen(imageProgram, vao);
		canvas->use();
		frameChanged = false;

		glFlush();
	}

	canvas->unuse();

	//put some draw calls here that won't be saved to the frame buffer

	canvas->use();
	
	glFlush();
}

//master keyboard control
void Engine::handleKeyboardInput(int key, int action)
{
	//Tool controls
	if (key == GLFW_KEY_P && action == GLFW_PRESS) { // p for color picker
		std::cout << "Color picker active" << std::endl;
		useTool(colorPicker);
	}
	else if (key == GLFW_KEY_F && action == GLFW_PRESS) { // f for filltool
		std::cout << "Fill tool active" << std::endl;
		useTool(fillTool);
	}
	else if (key == GLFW_KEY_X && action == GLFW_PRESS) { // x to select
		std::cout << "Clipboard active in select mode" << std::endl;
		useTool(clipBoard);
		clipBoard->setCopyMode();
	}
	else if (key == GLFW_KEY_V && action == GLFW_PRESS) { // v to paste
		std::cout << "Clipboard active in paste mode" << std::endl;
		useTool(clipBoard);
		clipBoard->setPasteMode();
	}

	
	//Brush controls
	else if (key == GLFW_KEY_1 && action == GLFW_PRESS) { // 1 for pencil
		std::cout << "Pencil active" << std::endl;
		useBrush(pencil);
	}
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS) { // 2 for eraser
		std::cout << "Eraser active" << std::endl;
		useBrush(eraser);
	}
	else if (key == GLFW_KEY_3 && action == GLFW_PRESS) { // 3 for caligraphy brush (forward)
		std::cout << "Caligraphy brush (forward) active" << std::endl;
		useBrush(caligFBrush);
	}
	else if (key == GLFW_KEY_4 && action == GLFW_PRESS) { // 4 for caligraphy brush (backward)
		std::cout << "Caligraphy brush (backward) active" << std::endl;
		useBrush(caligBBrush);
	}
	else if (key == GLFW_KEY_5 && action == GLFW_PRESS) { // 5 for airbrush
		std::cout << "Airbrush active" << std::endl;
		useBrush(airbrush);
	}
	else if (key == GLFW_KEY_6 && action == GLFW_PRESS) { // 6 for marker
		std::cout << "Marker active" << std::endl;
		useBrush(marker);
	}


	//Other controls
	else if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS) { // numpad + to increase size
		if (clipBoard->isActive()) {
			std::cout << "increased image size" << std::endl;
			clipBoard->increaseSize();
		}
		else if (activeBrush->isActive()) {
			std::cout << "increased brush size" << std::endl;
			activeBrush->increaseSize();
		}
	}
	else if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS) { // numpad - to decrease size
		if (clipBoard->isActive()) {
			std::cout << "decreased image size" << std::endl;
			clipBoard->decreaseSize();
		}
		else if (activeBrush->isActive()) {
			std::cout << "decreased brush size" << std::endl;
			activeBrush->decreaseSize();
		}
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
					break;
				case 2:
					ColorBox::backgroundColor = glm::vec3(r, g, b);
					break;
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
					break;
				case 2:
					ColorBox::backgroundColor = glm::vec3(r, g, b);
					break;
				}
			}
		}
		system("cls");
	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS) { // s to save
		if (InputManager::controlDown()) {
			std::cout << "Enter output file name : " << std::endl;
			std::getline(std::cin, saveName);
			saveImageToFile(saveDirectory + saveName);
			std::cout << "saved image as " << saveName << std::endl;
		}
		else {
			saveImageToFile(saveDirectory + saveName);
			std::cout << "Saved image" << std::endl;
		}
	}
	else if (key == GLFW_KEY_D && action == GLFW_PRESS) { // d to change save directory
		std::cout << "Enter new output directory : " << std::endl;
		std::getline(std::cin, saveDirectory);
		if (saveDirectory[saveDirectory.length() - 1] != '/' && saveDirectory[saveDirectory.length() - 1] != '\\') {
			//std::cout << "Directory does not end with file separator" << std::endl;
			saveDirectory += '\\';
		}
		std::cout << "Changed output directory to " << saveDirectory << std::endl;
	}
	else if (key == GLFW_KEY_N && action == GLFW_PRESS) { // n for empty canvas
		int choice;
		std::cout << "Are you sure you want to clear the canvas?" << std::endl << "Yes (1)" << std::endl << "No (2)" << std::endl;
		std::cin >> choice;
		if (choice == 1) {
			glClear(GL_COLOR_BUFFER_BIT);
		}
	}

}

void Engine::handleFileDrop(const char * path)
{
	int choice;
	std::cout << "Reload image (1)" << std::endl << "Copy to clipboard (2)" << std::endl << "Cancel (3)" << std::endl;
	std::cin >> choice;

	if (choice == 1) {
		std::cout << "reloading image" << std::endl;
		reloadImage(path);
	}
	else if (choice == 2) {
		std::cout << "loading image : " << std::string(path) << std::endl;
		clipBoard->loadImage(path);
		std::cout << "image loaded to clipboard" << std::endl;
	}
}
