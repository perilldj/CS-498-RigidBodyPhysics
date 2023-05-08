#pragma once

#include <glad/glad.h>
#include "VBO.h"

/*
	Class: VAO
	Description: This is a class to encapsulate the management of an OpenGL VAO, which defines how vertex data
				 setup in a vertices array.

*/

class VAO {
public:
	GLuint ID;
	VAO();

	void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	void Bind();
	void Unbind();
	void Delete();
};