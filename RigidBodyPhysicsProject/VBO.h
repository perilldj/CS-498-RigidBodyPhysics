#pragma once

#include <glad/glad.h>

/*
	Class: VBO
	Description: Encapsulates OpenGL VBO functionality for ease of use. Stores mesh data.
*/

class VBO {

public:
	
	GLuint ID;
	VBO(GLfloat* vertices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();

};