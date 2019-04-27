#pragma once

//Interface for easy toggle of tools
class Tool
{
public :
	virtual void use() = 0;
	virtual void unuse() = 0;
};
