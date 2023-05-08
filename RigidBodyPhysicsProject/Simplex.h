#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Simplex {
	int pointCount = 0;
	glm::vec3 a, b, c, d;
};