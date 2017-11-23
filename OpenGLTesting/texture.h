#pragma once

#include "stb_image.h"

#include <string>
#include <stdio.h>
#include <glad\glad.h>

using namespace std;


static const std::string TEXTURE_PATH = "../OpenGLTesting/res/textures/";


enum class TextureType {
	DIFFUSE,
	SPECULAR,
	DIFFUSE_AND_SPECULAR
};

struct Texture {
	GLuint id;
	TextureType type;
	string path;
};


void textureInit(Texture * texture, const char * textureName, TextureType type, bool flipY = false, bool isPng = false);
void textureUse(Texture * texture, GLuint slot);
void textureFree(Texture * texture);


void textureInit(Texture * texture, const char * textureName, TextureType type, bool flipY, bool isPng)
{
	texture->type = type;
	texture->path = TEXTURE_PATH + textureName;
	glGenTextures(1, &texture->id);
	glBindTexture(GL_TEXTURE_2D, texture->id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(flipY);
	unsigned char * data = stbi_load(texture->path.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, isPng ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("Failed to load texture!\n");
	}
	stbi_image_free(data);
}

void textureUse(Texture * texture, GLuint slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texture->id);
	glActiveTexture(GL_TEXTURE0);
}

void textureFree(Texture * texture)
{
	glDeleteTextures(1, &texture->id);
}
