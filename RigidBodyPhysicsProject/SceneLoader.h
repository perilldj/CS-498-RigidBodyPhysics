#pragma once

#include <vector>
#include <fstream>
#include <sstream>
#include <regex>
#include <string>
#include "Object.h"

enum CurrentStage {
	Name, Vertex, Index, Finalize
};

struct NameAttributes {
	bool Anchor = false;
	bool Processed = false;
};

/*

	Class: SceneLoader
	Description: Contains all the necessary functionality to setup a simulation scene or baked scene from a .obj or .bake file.

*/

class SceneLoader {

public:

	static bool LoadScene(wchar_t szFile[260], std::vector<Object>* objects, Shader* shader, std::vector<std::string>& strFile);

private:

	static glm::vec3 TransformVertices(std::vector<GLfloat>* vertices);
	static NameAttributes ReadNameAttribute(const std::string& line);
	static void ReadVertexAttribute(const std::string& line, std::vector<GLfloat>* vertices);
	static void ReadIndexAttribute(const std::string& line, std::vector<GLuint>* indices, int totalVertices);
	static void CalculateVertexNormals(GLfloat* vertices, GLuint* indices, int triangleCount, int vertexCount);

	static Triangle GetTriangleData(GLuint* indices, int index);
	static glm::vec3 GetVertexData(GLfloat* vertices, int index);
	static void SetNormalData(GLfloat* vertices, glm::vec3 normal, int index);

	static void LoadBakedData(std::vector<Object>* objects, std::ifstream* file);

};