#include <iostream>

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "Texture.h"


//int Texture::defaultMinFilter = GL_LINEAR_MIPMAP_LINEAR;
int Texture::defaultMinFilter = GL_LINEAR;
int Texture::defaultMagFilter = GL_LINEAR;

//in this constructor the filters used are the global filters
Texture::Texture(std::string texturePath)
{
	construct(texturePath, defaultMinFilter, defaultMagFilter);
}

//in this constructor the filters are passed as parameters
Texture::Texture(std::string texturePath, int minFilter, int magFilter)
{
	construct(texturePath, minFilter, magFilter);
}


void Texture::use()
{
	//Automatically assigns the texture to the fragment shader's active sampler
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::updateContents(int width, int height, void * pixels, int numChannels)
{
	glBindTexture(GL_TEXTURE_2D, texture);

	if (numChannels == 3) {
		//don't forget to pad if not using 1 byte unpack alignment
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	}
	else if (numChannels == 4) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::construct(std::string texturePath, int minFilter, int magFilter)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//wrapping options (currently unchangeable)
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	stbi_set_flip_vertically_on_load(true);

	int width, height, numComponents;
	unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &numComponents, 0);

	GLenum format;

	//Use number of components to determine color format
	if (numComponents == 1) {
		format = GL_RED;
	}
	else if (numComponents == 3) {
		format = GL_RGB;
	}
	else if (numComponents == 4) {
		format = GL_RGBA;
	}
	else {
		throw std::runtime_error("Unknown color format for texture : " + texturePath + "\n");
	}

	if (data != nullptr) {
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D); //don't need mipmap in this situation
	}
	else {			
		throw std::runtime_error("Failed to load texture : " + texturePath + "\n");
	}

	stbi_image_free(data);

	this->path = texturePath;
}

void Texture::setDefaultMinFilter(int filter)
{
	defaultMinFilter = filter;
}

void Texture::setDefaultMagFilter(int filter)
{
	defaultMagFilter = filter;
}
