#include "VAO.h"

VAO::VAO() {
	glGenVertexArrays(1, &ID);
	this->Bind();
}

/*
	Function: VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
	Description: Links an attribute to the VBO, to define how a vertex array is layed out.	
*/

void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

void VAO::Bind() {
	glBindVertexArray(ID);
}

void VAO::Unbind() {
	glBindVertexArray(0);
}

void VAO::Delete() {
	glDeleteVertexArrays(1, &ID);
}