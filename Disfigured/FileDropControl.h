#pragma once

class FileDropControl
{
public:
	virtual void handleFileDrop(const char* path) = 0;
};
