#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct CollisionResults {

	glm::vec3 collisionNormal = glm::vec3(1.0f, 0.0f, 0.0f);
	float collisionDepth = 1.0f;

};