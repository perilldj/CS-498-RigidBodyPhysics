#include "Object.h"
#include "Simplex.h"
#include "CollisionResults.h"
#include <limits>

#define MAX_ATTEMPTS 64

glm::vec3 TriangleCase(Simplex* simplex, glm::vec3 direction);
bool TetrahedronCase(Simplex* simplex, glm::vec3& direction);
glm::vec3 support(Object* obj1, Object* obj2, glm::vec3 direction);
glm::vec3 FurthestPoint(Object* obj, glm::vec3 direction);
glm::vec3 EPA(Simplex* simplex, Object* obj1, Object* obj2);

/*
	Function: CheckCollision(Object* obj1, Object* obj2, CollisionResults &collisionResults);
	Description: Checks to see if the two provided objects are intesecting with each other, returns true if they are intersecting,
			     and false otherwise. Additionally, extra information about the collision will be set in the collisionResults struct
				 which is passed by reference.

	SOURCE: This 3D collision algorithm was modified from the following link: https://github.com/kevinmoran/GJK/blob/master/GJK.h
*/

bool CheckCollision(Object* obj1, Object* obj2, CollisionResults &collisionResults) {

	Simplex simplex;

	glm::vec3 o1o2 = obj1->position - obj2->position;
	glm::vec3 dir = o1o2;
	glm::vec3 supportPoint = support(obj2, obj1, dir);
	simplex.c = supportPoint;
	dir = -simplex.c;

	supportPoint = support(obj2, obj1, dir);
	simplex.b = supportPoint;

	if (glm::dot(simplex.b, dir) < 0) {
		return false;
	}

	dir = glm::cross(glm::cross(simplex.c - simplex.b, -simplex.b), simplex.c - simplex.b);

	if (dir == glm::vec3(0.0f, 0.0f, 0.0f)) {
		dir = glm::cross(simplex.c - simplex.b, glm::vec3(1.0f, 0.0f, 0.0f));
		if (dir == glm::vec3(0.0f, 0.0f, 0.0f))
			dir = glm::cross(simplex.c - simplex.b, glm::vec3(0.0f, 0.0f, -1.0f));
	}

	simplex.pointCount = 2;

	int attempts = MAX_ATTEMPTS;
	while (attempts > 0) {
		
		supportPoint = support(obj2, obj1, dir);
		simplex.a = supportPoint;
		if (glm::dot(simplex.a, dir) < 0) {
			return false;
		}

		simplex.pointCount++;
		if (simplex.pointCount == 3) {
			dir = TriangleCase(&simplex, dir);
		} else if (TetrahedronCase(&simplex, dir)) {
			glm::vec3 mtv = EPA(&simplex, obj1, obj2);
			collisionResults.collisionNormal = glm::normalize(mtv);
			collisionResults.collisionDepth = glm::length(mtv);
			return true;
		}

		attempts--;

	}

	std::cout << attempts << std::endl;
	return false;

}

glm::vec3 TriangleCase(Simplex* simplex, glm::vec3 direction) {
	
	glm::vec3 normal = glm::cross(simplex->b - simplex->a, simplex->c - simplex->a);
	glm::vec3 AO = glm::vec3(0.0f, 0.0f, 0.0f) - simplex->a;

	simplex->pointCount = 2;
	if (glm::dot(glm::cross(simplex->b - simplex->a, normal), AO) > 0) {
		simplex->c = simplex->a;
		return glm::cross(glm::cross(simplex->b - simplex->a, AO), simplex->b - simplex->a);
	}

	if (glm::dot(glm::cross(normal, simplex->c - simplex->a), AO) > 0) {
		simplex->b = simplex->a;
		return glm::cross(glm::cross(simplex->c - simplex->a, AO), simplex->c - simplex->a);
	}

	simplex->pointCount = 3;
	if (glm::dot(normal, AO) > 0) {
		simplex->d = simplex->c;
		simplex->c = simplex->b;
		simplex->b = simplex->a;
		return normal;
	}

	simplex->d = simplex->b;
	simplex->b = simplex->a;
	return -normal;

}

bool TetrahedronCase(Simplex* simplex, glm::vec3& direction) {
	
	glm::vec3 ABC = glm::cross(simplex->b - simplex->a, simplex->c - simplex->a);
	glm::vec3 ACD = glm::cross(simplex->c - simplex->a, simplex->d - simplex->a);
	glm::vec3 ADB = glm::cross(simplex->d - simplex->a, simplex->b - simplex->a);
	glm::vec3 AO = glm::vec3(0.0f, 0.0f, 0.0f) - simplex->a;
	simplex->pointCount = 3;
	
	if (glm::dot(ABC, AO) > 0) {
		simplex->d = simplex->c;
		simplex->c = simplex->b;
		simplex->b = simplex->a;
		direction = ABC;
		return false;
	}
	
	if (glm::dot(ACD, AO) > 0) {
		simplex->b = simplex->a;
		direction = ACD;
		return false;
	}
	
	if (glm::dot(ADB, AO) > 0) {
		simplex->c = simplex->d;
		simplex->d = simplex->b;
		simplex->b = simplex->a;
		direction = ADB;
		return false;
	}

	return true;
}

glm::vec3 support(Object* obj1, Object* obj2, glm::vec3 direction) {
	glm::vec3 p1 = FurthestPoint(obj1, direction);
	glm::vec3 p2 = FurthestPoint(obj2, -direction);
	return p1 - p2;
}

glm::vec3 FurthestPoint(Object* obj, glm::vec3 direction) {
	glm::vec3 result = glm::vec3(0.0f, 0.0f, 0.0f);
	float max = -INFINITY;
	int index = 0;
	for (int i = 0; i < obj->GetVertexCount(); i++) {
		glm::vec3 p = obj->GetVertexData(i);
		//std::cout << p.x << " " << p.y << " " << p.z << std::endl;
		float dot = glm::dot(p, direction);
		if (dot > max) {
			max = dot;
			result = p;
		}
	}
	return obj->ConvertPointToWorldSpace(result);
}

/*
	Private EPA function, again modified from https://github.com/kevinmoran/GJK/blob/master/GJK.h
*/

glm::vec3 EPA(Simplex* simplex, Object* obj1, Object* obj2) {

	glm::vec3 faces[MAX_ATTEMPTS][4]; // A face contains 3 vertices and it's normal.

	// Add faces from simplex.
	faces[0][0] = simplex->a;
	faces[0][1] = simplex->b;
	faces[0][2] = simplex->c;
	faces[0][3] = glm::normalize(glm::cross(simplex->b - simplex->a, simplex->c - simplex->a)); //ABC
	faces[1][0] = simplex->a;
	faces[1][1] = simplex->c;
	faces[1][2] = simplex->d;
	faces[1][3] = glm::normalize(glm::cross(simplex->c - simplex->a, simplex->d - simplex->a)); //ACD
	faces[2][0] = simplex->a;
	faces[2][1] = simplex->d;
	faces[2][2] = simplex->b;
	faces[2][3] = glm::normalize(glm::cross(simplex->d - simplex->a, simplex->b - simplex->a)); //ADB
	faces[3][0] = simplex->b;
	faces[3][1] = simplex->d;
	faces[3][2] = simplex->c;
	faces[3][3] = glm::normalize(glm::cross(simplex->d - simplex->b, simplex->c - simplex->b)); //BDC

	int faceCount = 4;
	int closestFaceIndex;

	int attempts = MAX_ATTEMPTS;
	while (attempts > 0) {
		
		float min = glm::dot(faces[0][0], faces[0][3]);
		closestFaceIndex = 0;
		for (int i = 1; i < faceCount; i++) {
			float distance = glm::dot(faces[i][0], faces[i][3]);
			if (distance < min) {
				min = distance;
				closestFaceIndex = i;
			}
		}

		glm::vec3 direction = faces[closestFaceIndex][3];
		glm::vec3 supportPoint = support(obj2, obj1, direction);

		if (glm::dot(supportPoint, direction) - min < 0.0001)
			return faces[closestFaceIndex][3] * glm::dot(supportPoint, direction);

		glm::vec3 looseEdges[MAX_ATTEMPTS / 2][2];
		int looseEdgeCount = 0;

		for (int i = 0; i < faceCount; i++) {
			if (glm::dot(faces[i][3], supportPoint - faces[i][0]) > 0) {
				for (int j = 0; j < 3; j++) {
					glm::vec3 currentEdge[2] = { faces[i][j], faces[i][(j + 1) % 3] };
					bool foundEdge = false;
					for (int k = 0; k < looseEdgeCount; k++) {
						if (looseEdges[k][1] == currentEdge[0] && looseEdges[k][0] == currentEdge[1]) {
							looseEdges[k][0] = looseEdges[looseEdgeCount - 1][0];
							looseEdges[k][1] = looseEdges[looseEdgeCount - 1][1];
							looseEdgeCount--;
							foundEdge = true;
							k = looseEdgeCount;
						}
					}

					if (!foundEdge) {
						if (looseEdgeCount >= MAX_ATTEMPTS / 2)
							break;
						looseEdges[looseEdgeCount][0] = currentEdge[0];
						looseEdges[looseEdgeCount][1] = currentEdge[1];
						looseEdgeCount++;
					}

				}
			}

			faces[i][0] = faces[faceCount - 1][0];
			faces[i][1] = faces[faceCount - 1][1];
			faces[i][2] = faces[faceCount - 1][2];
			faces[i][3] = faces[faceCount - 1][3];
			faceCount--;
			i--;

		}

		for (int i = 0; i < looseEdgeCount; i++) {
			if (faceCount >= MAX_ATTEMPTS)
				break;
			faces[faceCount][0] = looseEdges[i][0];
			faces[faceCount][1] = looseEdges[i][1];
			faces[faceCount][2] = supportPoint;
			faces[faceCount][3] = glm::normalize(glm::cross(looseEdges[i][0] - looseEdges[i][1], looseEdges[i][0] - supportPoint));

			float bias = 0.000001f;

			if (glm::dot(faces[faceCount][0], faces[faceCount][3]) + bias < 0) {
				glm::vec3 temp = faces[faceCount][0];
				faces[faceCount][0] = faces[faceCount][1];
				faces[faceCount][1] = temp;
				faces[faceCount][3] = -faces[faceCount][3];
			}

			faceCount++;

		}

		attempts--;

	}

	return faces[closestFaceIndex][3] * glm::dot(faces[closestFaceIndex][0], faces[closestFaceIndex][3]);

}