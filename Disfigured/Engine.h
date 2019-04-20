#pragma once

#include <string>

#include "Window.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Pencil.h"
#include "Eraser.h"
#include "KeyboardControl.h"
#include "ColorBox.h"
#include "FillTool.h"
#include "ColorPicker.h"


class Engine : public KeyboardControl
{
public:

	void start();

private:
	GLuint vao;
	GLuint vbo;

	Window* window;
	ShaderProgram* imageProgram;
	ShaderProgram* lineProgram;
	ShaderProgram* pencilProgram;
	ShaderProgram* eraserProgram;
	Texture* image;
	Pencil* pencil;
	Eraser* eraser;
	ColorBox* colorBox;
	FillTool* fillTool;
	ColorPicker* colorPicker;

	bool shouldDrawImage;

	void init();
	void mainLoop();
	void cleanup();

	Texture* loadImage(std::string path);
	void saveImage(std::string path);
	void drawImage(Texture* image);

	void renderFrame();

	void handleKeyboardInput(int key, int action) override;
};
