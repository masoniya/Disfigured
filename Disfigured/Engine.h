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
#include "AirBrush.h"
#include "Marker.h"
#include "ClipBoard.h"
#include "Tool.h"


class Engine : public KeyboardControl
{
public:

	void start();

private:

	GLuint vao;
	GLuint vbo;

	Window* window;
	Texture* image;
	ColorBox* colorBox;
	FillTool* fillTool;
	ColorPicker* colorPicker;
	ClipBoard* clipBoard;

	//shaders
	ShaderProgram* imageProgram;
	ShaderProgram* lineProgram;
	ShaderProgram* pencilProgram;
	ShaderProgram* eraserProgram;
	ShaderProgram* caligFProgram;
	ShaderProgram* caligBProgram;
	ShaderProgram* airbrushProgram;
	ShaderProgram* markerProgram;

	//available brushes
	Brush* pencil;
	Brush* eraser;
	Brush* caligFBrush;
	Brush* caligBBrush;

	AirBrush* airbrush; //separate class because it has different behavior
	Marker* marker; //separate class because of weird shape and opacity behavior

	Brush* activeBrush;
	Tool* activeTool;


	bool imageLoaded;
	bool frameSaved;

	void init();
	void mainLoop();
	void cleanup();

	void renderFrame();

	void initShaders();
	void initBrushes();
	void initTools();
	void initVertexData();

	void registerBrush(Brush* brush);
	void useBrush(Brush* brush);
	void useTool(Tool* tool);

	Texture* loadImage(std::string path);
	void saveImageToFile(std::string path);
	void drawImage(Texture* image);
	void saveFrameToImage();

	void handleKeyboardInput(int key, int action) override;
};
