#pragma once

#include "stb_image.h"

#include <string>
#include <stdio.h>
#include <glad\glad.h>


enum class TextureType {
	DIFFUSE,
	SPECULAR,
	DIFFUSE_AND_SPECULAR
};

struct Texture {
	GLuint id;
	TextureType type;
	std::string path;
};


void textureInit(Texture * texture, const char * textureName, TextureType type, bool flipY = false, bool isPng = false);
void textureUse(Texture * texture, GLuint slot);
void textureFree(Texture * texture);
