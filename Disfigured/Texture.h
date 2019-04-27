#pragma once

#include <string>


class Texture
{
public:
	Texture(std::string texturePath);
	Texture(std::string texturePath, int minFilter, int magFilter);

	void use();

	//upload new pixel data to the texture
	void updateContents(int width, int height, void * pixels, int numChannels);

	//load new image and upload its data to the texture
	void reload(std::string texturePath);

	static void setDefaultMinFilter(int filter);
	static void setDefaultMagFilter(int filter);

private:

	void construct(std::string texturePath, int minFilter, int magFilter);

	uint32_t texture;
	std::string path;

	static int defaultMinFilter;
	static int defaultMagFilter;
};
