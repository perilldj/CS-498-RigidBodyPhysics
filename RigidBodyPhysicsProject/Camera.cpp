#include "Camera.h"

// Constructor
Camera::Camera(int width, int height, glm::vec3 position) {
	this->width = width;
	this->height = height;
	this->position = position;
}

/*
	Function: void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform 
	Description: Calculates the camera view transformation and uploads it to the provided shader through the provided uniform.
*/

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform) {
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(position, position + direction, up);

	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(view));
}

/*
	Function: void Camera::Inputs(GLFWwindow* window)
	Description: Gets input from the user, and ajusts the position, direction, and speed based on those inputs.
*/

void Camera::Inputs(GLFWwindow* window) {
	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		position += speed * direction;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		position += speed * -glm::normalize(glm::cross(direction, up));
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		position += speed * -direction;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		position += speed * glm::normalize(glm::cross(direction, up));
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		position += speed * up;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		position += speed * -up;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		speed = 0.5f;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
		speed = 0.2f;
	}

}

void Camera::MouseCallback(GLFWwindow* window, double xpos, double ypos) {

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {


		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		float rotx = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		glm::vec3 newDirection = glm::rotate(direction, glm::radians(-rotx), glm::normalize(glm::cross(direction, up)));

		if (!(glm::angle(newDirection, up) <= glm::radians(5.0f) || glm::angle(direction, -up) <= glm::radians(5.0f))) {
			direction = newDirection;
		}

		direction = glm::rotate(direction, glm::radians(-rotY), up);
		glfwSetCursorPos(window, (width / 2), (height / 2));

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

}

