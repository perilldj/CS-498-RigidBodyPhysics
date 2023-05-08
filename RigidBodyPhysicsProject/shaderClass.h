#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <iostream>
#include <cerrno>

std::string get_file_contents(const char* filename);

/*
	Class: Shader
	Description: Provides encapsulating functionality to create, compile, and use OpenGL shaders.
*/

class Shader {

public:
	GLuint ID;
	Shader(const char* vertexFile, const char* fragmentFile);
	
	void Activate();
	void Delete();

private:
	void compileErrors(unsigned int shader, const char* type);

};