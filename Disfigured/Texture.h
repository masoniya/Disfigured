#pragma once

#include <string>


class Texture
{
public:
	Texture(std::string texturePath);
	Texture(std::string texturePath, int minFilter, int magFilter);

	void use();

	void updateContents(int width, int height, void * pixels, int numChannels);

	static void setDefaultMinFilter(int filter);
	static void setDefaultMagFilter(int filter);

	uint32_t texture;
	std::string path;

private:
	void construct(std::string texturePath, int minFilter, int magFilter);

	static int defaultMinFilter;
	static int defaultMagFilter;
};
