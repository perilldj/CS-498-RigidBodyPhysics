#pragma once

#include <glad/glad.h>
#include <stb/stb_image.h>

#include "shaderClass.h"

/*
	Class: Texture
	Description: Allows for the loading, processing, and storage of textures, along with applying a texture to a shader.
*/

class Texture {
public:
	GLuint ID;
	GLenum type;
	Texture(const char* image, GLenum texType, GLenum slot, GLenum formal, GLenum pixelType);
	 
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	void Bind();
	void Unbind();
	void Delete();
};