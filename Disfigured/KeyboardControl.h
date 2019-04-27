#pragma once

//Interface to get Keyboard input
class KeyboardControl
{
public:
	virtual void handleKeyboardInput(int key, int action) = 0;
};
