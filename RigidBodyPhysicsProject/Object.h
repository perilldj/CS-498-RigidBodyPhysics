#pragma once

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "VAO.h"
#include "EBO.h"
#include "Texture.h"
#include "shaderClass.h"
#include "Triangle.h"

/*

	Class: Object
	Description: A 3D object which can be rendered to the world, primarily used for physics collision and simulation, this object
				 encapsulates all OpenGL functions needed to render a 3D object to the world, so other parts of the program do not have
				 to worry about it.

				 Additionally, the object contains the core of the linear physics simulaton on the resolve and integrate functions.
	
*/

class Object {

public:

	int ID = -1;

	std::vector<glm::vec3> bakedPositions;

	// Transformation variables
	glm::vec3 originalPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

	Object(GLfloat* meshData, int meshSize, GLuint* indicesData, int indicesSize, Shader* shader);

	bool anchored = false;

	float gravity = 15.0f;
	glm::vec3 gravityForce = glm::vec3(0.0f, -gravity, 0.0f);
	glm::vec3 cumulativeForce = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	void Integrate(float dt);
	void Resolve(std::vector<Object>* objects, float dt);
	void AddForce(glm::vec3 force);

	int GetVertexCount();
	glm::vec3 GetVertexData(int index);

	glm::vec3 ConvertPointToWorldSpace(glm::vec3 point);

	void draw();
	void setShader(Shader* shader);
	void destroy();

	static GLfloat pyramidMeshVertices[];
	static int pyramidMeshVerticesSize;
	static GLuint pyramidMeshIndices[];
	static int pyramidMeshIndicesSize;

private:

	GLfloat* meshData;
	GLuint* indicesData;

	int meshSize;
	int indicesSize;

	Shader* shader;

	VAO VAO;
	VBO VBO;
	EBO EBO;

	// Transformation matrices
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 proj = glm::mat4(1.0f);

};