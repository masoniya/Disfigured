#pragma once

//Interface to get file drop input
class FileDropControl
{
public:
	virtual void handleFileDrop(const char* path) = 0;
};
