#pragma once

#include <glad/glad.h>

/*
	Class: EBO
	Description: Class to encapsulate an OpenGL EBO for ease of use. Stores indices information for a mesh.
*/

class EBO {
public:
	GLuint ID;
	EBO(GLuint* indices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};