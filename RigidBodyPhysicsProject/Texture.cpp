#include "Texture.h"

// Constructor
Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum formal, GLenum pixelType) {
	type = texType;

	// Load the textutre from the provided filepath
	int widthImg, heightImg, numColch;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColch, 0);

	// Generate a texture on opengl
	glGenTextures(1, &ID);
	glActiveTexture(slot);
	glBindTexture(texType, ID);

	// Set scalling parameters
	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Set wrapping parameters
	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Specify a 2-dimensional image
	glTexImage2D(texType, 0, GL_RGB, widthImg, heightImg, 0, GL_RGB, pixelType, bytes);
	glGenerateMipmap(texType);

	// Free image data from stbi, it's loaded into the GPU now
	stbi_image_free(bytes);

	// Unbind texture object
	glBindTexture(texType, 0);

}

/*
	Function: void Texture::texUnit(Shader& shader, const char* uniform , GLuint unit)
	Description: sets a texture of a shader to a specific texture, from an opengl texture slot
*/

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit) {
	GLuint tex0Uni = glGetUniformLocation(shader.ID, uniform);
	shader.Activate();
	glUniform1i(tex0Uni, unit);
}

void Texture::Bind() {
	glBindTexture(type, ID);
}

void Texture::Unbind() {
	glBindTexture(type, 0);
}

void Texture::Delete() {
	glDeleteTextures(1, &ID);
}
