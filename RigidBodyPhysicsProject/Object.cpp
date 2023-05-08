#include "Object.h"
#include "GJK.h"
#include "CollisionResults.h"
#include <iostream>


// Pyramid Mesh information

GLfloat Object::pyramidMeshVertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
	-0.3f, 0.0f,  0.3f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	-0.3f, 0.0f, -0.3f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.3f, 0.0f, -0.3f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	 0.3f, 0.0f,  0.3f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
};

int Object::pyramidMeshVerticesSize = 40 * sizeof(GLfloat);

GLuint Object::pyramidMeshIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};

int Object::pyramidMeshIndicesSize = 18 * sizeof(GLuint);

// Constructor, creates and sets up the VAO, VBO, and EBO
Object::Object(GLfloat* meshData, int meshSize, GLuint* indicesData, int indicesSize, Shader* shader)
	: VAO(), VBO(meshData, meshSize), EBO(indicesData, indicesSize) {

	this->meshSize = meshSize;
	this->indicesSize = indicesSize;
	this->meshData = meshData;
	this->indicesData = indicesData;
	this->shader = shader;

	this->anchored = false;

	// Setup attributes in the VAO, setup positional data, color data, and texture coordinate data
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO.LinkAttrib(VBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

void Object::Integrate(float dt) {
	if(anchored)
		return;
	velocity += cumulativeForce * dt;
	position += velocity * dt;
	cumulativeForce = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Object::Resolve(std::vector<Object>* objects, float dt) {

	for (int i = 0; i < objects->size(); i++) {
		if (i == ID)
			continue;

		CollisionResults collisionResults;
		if (CheckCollision(this, &(*objects)[i], collisionResults)) {
			
			if (std::isnan(collisionResults.collisionDepth) || std::isnan(collisionResults.collisionNormal.x) || std::isnan(collisionResults.collisionNormal.y) || std::isnan(collisionResults.collisionNormal.z))
				return;
			//std::cout << collisionResults.collisionDepth << std::endl;

			if (!anchored) {
				this->position += collisionResults.collisionNormal * (collisionResults.collisionDepth) * 0.5f;
				glm::vec3 relativeVelocity = (*objects)[i].velocity - this->velocity;
				float e = 0.5f;
				float j = (-1.0f - e) * glm::dot(relativeVelocity, collisionResults.collisionNormal);
				j /= glm::dot(collisionResults.collisionNormal, collisionResults.collisionNormal) * (1.0f / 1.0f) + (1.0f / 1.0f);
				this->AddForce(-((j / 1.0f) * collisionResults.collisionNormal) * 60.0f);
				(*objects)[i].AddForce(((j / 1.0f) * collisionResults.collisionNormal) * 60.0f);
			}

			if (!(*objects)[i].anchored) {
				//(*objects)[i].AddForce(-collisionResults.collisionNormal * (collisionResults.collisionDepth));
				//(*objects)[i].position += -collisionResults.collisionNormal * (collisionResults.collisionDepth / 2.0f);
			}
		}
		else {
			//std::cout << "no collision" << std::endl;
		}
	}

	AddForce(gravityForce);

}

void Object::AddForce(glm::vec3 force) {
	cumulativeForce += force;
}

int Object::GetVertexCount() {
	return (meshSize / 8) / sizeof(GLfloat);
}

glm::vec3 Object::GetVertexData(int index) {
	glm::vec3 vertex = glm::vec3(0.0f, 0.0f, 0.0f);
	vertex.x = (float)meshData[index * 8];
	vertex.y = (float)meshData[(index * 8) + 1];
	vertex.z = (float)meshData[(index * 8) + 2];
	return vertex;
}

glm::vec3 Object::ConvertPointToWorldSpace(glm::vec3 point) {
	glm::vec4 point4 = glm::vec4(point, 0.0f);
	glm::mat4 rotate = glm::mat4(1.0f);
	rotate = glm::rotate(rotate, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	rotate = glm::rotate(rotate, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	rotate = glm::rotate(rotate, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	point4 = (rotate * point4);
	point = glm::vec3(point4);
	point += position;
	return point;
}

/*

	Function: draw();
	Description: Applies transformations and draws the 3D object to the screen.

*/

void Object::draw() {
	
	shader->Activate();

	// Reset tranformation matrices
	model = glm::mat4(1.0f);
	view = glm::mat4(1.0f);
	proj = glm::mat4(1.0f);

	// Calculate transformation matrices from location and roation ( scale TBA ).
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	view = glm::translate(view, position);
	proj = glm::perspective(glm::radians(80.0f), (float)(1920.0f / 1080.0f), 0.1f, 1000.0f);

	// Find transformation matrices uniforms and upload the transformations to the shader
	int modelLoc = glGetUniformLocation(shader->ID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	int viewLoc = glGetUniformLocation(shader->ID, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	int projLoc = glGetUniformLocation(shader->ID, "proj");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

	int colorLoc = glGetUniformLocation(shader->ID, "objColor");
	glUniform4fv(colorLoc, 1, glm::value_ptr(color));

	// Bind the VAO
	VAO.Bind();

	// Execute draw call
	glDrawElements(GL_TRIANGLES, indicesSize / sizeof(int), GL_UNSIGNED_INT, 0);

}

/*
	Function: void setShader(Shader* shader) 
	Description: Sets the shader to be used when drawing the object to the screen.
*/

void Object::setShader(Shader* shader) {
	this->shader = shader;
}

void Object::destroy() {
	VAO.Delete();
	VBO.Delete();
	EBO.Delete();
}
