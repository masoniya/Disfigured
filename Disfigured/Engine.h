#pragma once

#include <string>

#include "Window.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "KeyboardControl.h"
#include "ColorBox.h"
#include "FillTool.h"
#include "ColorPicker.h"
#include "Brush.h"


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
	ShaderProgram* caligFProgram;
	ShaderProgram* caligBProgram;
	Texture* image;
	ColorBox* colorBox;
	FillTool* fillTool;
	ColorPicker* colorPicker;

	//available brushes
	Brush* pencil;
	Brush* eraser;
	Brush* caligFBrush;
	Brush* caligBBrush;

	Brush* activeBrush;

	bool imageLoaded;
	bool frameSaved;

	void init();
	void mainLoop();
	void cleanup();

	void registerBrush(Brush* brush);
	void useBrush(Brush* brush);

	Texture* loadImage(std::string path);
	void saveImage(std::string path);
	void drawImage(Texture* image);
	void saveFrame();

	void renderFrame();

	void handleKeyboardInput(int key, int action) override;
};
