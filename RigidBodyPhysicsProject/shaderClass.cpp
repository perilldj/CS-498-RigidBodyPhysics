#include "shaderClass.h"


/*
	Function: std::string get_file_contents
	Description: Loads a file from a filepath
*/

std::string get_file_contents(const char* filename) {
	std::ifstream in(filename, std::ios::binary);		// Create an input file stream
	if (in) {											// Check if the file has opened properly
		std::string contents;
		in.seekg(0, std::ios::end);						// Find and move to end of file
		contents.resize(in.tellg());					// Resize contents string to the size of the ifstream, which is its current index at the end of the file
		in.seekg(0, std::ios::beg);						// Move back to the beggining of the file
		in.read(&contents[0], contents.size());			// Read the entire file into the contents string
		in.close();										// Close file stream
		return contents;								// Return loaded shader
	}
	throw(errno);										// If the shader did not load, throw an error
}

// Constructor
Shader::Shader(const char* vertexFile, const char* fragmentFile) {
	
	// Load provided vertex and fragment shader files
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	// Convert std::string to const char*
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// Create a vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Set shader source code
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Compile vertex shader
	glCompileShader(vertexShader);
	compileErrors(vertexShader, "VERTEX");

	// Create a fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Set fragment source code
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// Compile fragment shader
	glCompileShader(fragmentShader);
	compileErrors(fragmentShader, "FRAGMENT");

	// Create shader program
	ID = glCreateProgram();
	// Attach vertex and fragment shaders
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	// Link shader program
	glLinkProgram(ID);
	compileErrors(ID, "PROGRAM");

	// Remove vertex and fragment shader as they're compiled in the main shader program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

void Shader::Activate() {
	glUseProgram(ID);
}

void Shader::Delete() {
	glDeleteProgram(ID);
}

/* 
	Function: void Shader::compileErrors(unsigned int shader, const char* type)
	Description: Checks if a shader has successfully compiles and linked.
				 If it failed, it will print that it failed, and if it failed compiling or linking.
*/
void Shader::compileErrors(unsigned int shader, const char* type) {
	GLint hasCompiled;
	char infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for: " << type << "\n" << std::endl;
		}
	} else {
		glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for: " << type << "\n" << std::endl;
		}
	}
}
