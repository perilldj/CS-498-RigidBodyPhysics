#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shaderClass.h"

/*

	Class: Camera
	Description: Allows the user to look and move around the world.

*/

class Camera {

public:

	glm::vec3 position;
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	int width;
	int height;

	float speed = 1.0f;
	float sensitivity = 50.0f;

	Camera(int width, int height, glm::vec3 position);

	void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform);
	void Inputs(GLFWwindow* window);

	void MouseCallback(GLFWwindow* window, double xpos, double ypos);

};
 