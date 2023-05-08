#include "SceneLoader.h"

/*
	Function: SceneLoader::LoadScene(wchar_t szFile[260], std::vector<Object>* objects, Shader* shader, std::vector<std::string> &strFile)
	Description: Loads a various scene, file can be a .obj or .bake file. Will perform necessary functions to get both up and running.
*/

bool SceneLoader::LoadScene(wchar_t szFile[260], std::vector<Object>* objects, Shader* shader, std::vector<std::string> &strFile) {

	std::ifstream file;
	file.open(szFile);

	CurrentStage currentStage = CurrentStage::Name;

	NameAttributes objectAttributes;
	std::vector<GLfloat> objectVertices;
	std::vector<GLuint> objectIndices;
	int totalVertices = 0;

	std::string line;
	while (std::getline(file, line)) {

		// OBJ reading should occur in 4 stages, name, vertex, index, and finalize.
		
		// The name will serve for physics based attributes, naming your object as Anchor will anchor
		// an object, and will not move throughout the simulation.

		strFile.push_back(line);

		if (line[0] == '#' || line[0] == 's')
			continue;

		// Checks if we need to switch to loading baked data
		if (line[0] == '!') {
			glm::vec3 position = TransformVertices(&objectVertices);
			GLfloat* vertices = new GLfloat[objectVertices.size()];
			std::copy(objectVertices.begin(), objectVertices.end(), vertices); // Hacky copy :(
			GLuint* indices = new GLuint[objectIndices.size()];
			std::copy(objectIndices.begin(), objectIndices.end(), indices); // Hacky copy :(
			CalculateVertexNormals(vertices, indices, objectIndices.size() / 3 / 2, objectVertices.size() / 8);
			objects->push_back(Object(vertices, objectVertices.size() * sizeof(GLfloat), indices, objectIndices.size() * sizeof(GLuint), shader));
			(*objects)[objects->size() - 1].position = position;
			(*objects)[objects->size() - 1].originalPosition = position;
			(*objects)[objects->size() - 1].ID = objects->size() - 1;
			(*objects)[objects->size() - 1].anchored = objectAttributes.Anchor;
			if (!objectAttributes.Anchor)
				(*objects)[objects->size() - 1].color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			totalVertices += objectVertices.size() / 8;
			objectVertices.clear();
			objectIndices.clear();
			LoadBakedData(objects, &file);
			return true;
		}

		// Switched stages
		if (currentStage == CurrentStage::Name && line[0] == 'v')
			currentStage = CurrentStage::Vertex;
		else if (currentStage == CurrentStage::Vertex && line[0] == 'f')
			currentStage = CurrentStage::Index;
		else if (currentStage == CurrentStage::Index && line[0] != 'f')
			currentStage = CurrentStage::Finalize;

		switch (currentStage) {
		case CurrentStage::Name:
			objectAttributes = ReadNameAttribute(line);
			break;
		case CurrentStage::Vertex:
			ReadVertexAttribute(line, &objectVertices);
			break;
		case CurrentStage::Index:
			ReadIndexAttribute(line, &objectIndices, totalVertices);
			break;
		case CurrentStage::Finalize:
			glm::vec3 position = TransformVertices(&objectVertices);
			GLfloat* vertices = new GLfloat[objectVertices.size()];
			std::copy(objectVertices.begin(), objectVertices.end(), vertices); // Hacky copy :(
			GLuint* indices = new GLuint[objectIndices.size()];
			std::copy(objectIndices.begin(), objectIndices.end(), indices); // Hacky copy :(
			CalculateVertexNormals(vertices, indices, objectIndices.size() / 3 / 2, objectVertices.size() / 8);
			objects->push_back(Object(vertices, objectVertices.size() * sizeof(GLfloat), indices, objectIndices.size() * sizeof(GLuint), shader));
			(*objects)[objects->size() - 1].position = position;
			(*objects)[objects->size() - 1].originalPosition = position;
			(*objects)[objects->size() - 1].ID = objects->size() - 1;
			(*objects)[objects->size() - 1].anchored = objectAttributes.Anchor;
			if (!objectAttributes.Anchor)
				(*objects)[objects->size() - 1].color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			totalVertices += objectVertices.size() / 8;
			objectVertices.clear();
			objectIndices.clear();
			objectAttributes = ReadNameAttribute(line);
			if (!objectAttributes.Processed)
				return true;
			currentStage = CurrentStage::Vertex;
			break;
		}

	}

	glm::vec3 position = TransformVertices(&objectVertices);
	GLfloat* vertices = new GLfloat[objectVertices.size()];
	std::copy(objectVertices.begin(), objectVertices.end(), vertices); // Hacky copy :(
	GLuint* indices = new GLuint[objectIndices.size()];
	std::copy(objectIndices.begin(), objectIndices.end(), indices); // Hacky copy :(
	CalculateVertexNormals(vertices, indices, objectIndices.size() / 3 / 2, objectVertices.size() / 8);
	objects->push_back(Object(vertices, objectVertices.size() * sizeof(GLfloat), indices, objectIndices.size() * sizeof(GLuint), shader));
	(*objects)[objects->size() - 1].position = position;
	(*objects)[objects->size() - 1].originalPosition = position;
	(*objects)[objects->size() - 1].ID = objects->size() - 1;
	(*objects)[objects->size() - 1].anchored = objectAttributes.Anchor;
	if (!objectAttributes.Anchor)
		(*objects)[objects->size() - 1].color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	totalVertices += objectVertices.size() / 8;
	objectVertices.clear();
	objectIndices.clear();

	return true;

}

glm::vec3 SceneLoader::TransformVertices(std::vector<GLfloat>* vertices) {

	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	int vertexCount = vertices->size() / 8;
	for (int i = 0; i < vertexCount; i++) {
		position.x += (float)(vertices->at((i * 8)));
		position.y += (float)(vertices->at((i * 8) + 1));
		position.z += (float)(vertices->at((i * 8) + 2));
	}

	position /= (float)vertexCount;

	for (int i = 0; i < vertexCount; i++) {
		(*vertices)[(i * 8)] -= (GLfloat)position.x;
		(*vertices)[(i * 8) + 1] -= (GLfloat)position.y;
		(*vertices)[(i * 8) + 2] -= (GLfloat)position.z;
	}

	return position;

}

NameAttributes SceneLoader::ReadNameAttribute(const std::string& line) {
	NameAttributes attributes;
	if (line.empty())
		return attributes;
	std::stringstream ss;
	ss << line;
	std::string token;
	ss >> token >> token;
	std::regex pattern("\\.[0-9]+");
	token = std::regex_replace(token, pattern, "");
	if (token == "Anchor")
		attributes.Anchor = true;
	else
		attributes.Anchor = false;
	attributes.Processed = true;
	return attributes;
}



void SceneLoader::ReadVertexAttribute(const std::string& line, std::vector<GLfloat>* vertices) {
	std::stringstream ss;
	float x, y, z;
	std::string temp;
	ss << line;
	ss >> temp >> x >> y >> z;
	vertices->push_back(static_cast<GLfloat>(x));
	vertices->push_back(static_cast<GLfloat>(y));
	vertices->push_back(static_cast<GLfloat>(z));
	for(int i = 0; i < 5; i++)
		vertices->push_back(0.0f);
}

void SceneLoader::ReadIndexAttribute(const std::string& line, std::vector<GLuint>* indices, int totalVertices) {
	std::stringstream ss;
	int i1, i2, i3, i4 = -1;
	std::string temp;
	ss << line;
	ss >> temp >> i1 >> i2 >> i3 >> i4;
	bool isTri = false;
	if (i4 == -1)
		isTri = true;;
	i1 -= totalVertices + 1; i2 -= totalVertices + 1; i3 -= totalVertices + 1; i4 -= totalVertices + 1;
	indices->push_back(static_cast<GLuint>(i1));
	indices->push_back(static_cast<GLuint>(i2));
	indices->push_back(static_cast<GLuint>(i3));
	if (isTri)
		return;
	indices->push_back(static_cast<GLuint>(i1));
	indices->push_back(static_cast<GLuint>(i3));
	indices->push_back(static_cast<GLuint>(i4));
}

void SceneLoader::CalculateVertexNormals(GLfloat* vertices, GLuint* indices, int triangleCount, int vertexCount) {

	std::vector<glm::vec3> triangleNormals;
	std::vector<glm::vec3> vertexNormals;
	std::vector<int> vertexNormalsCount;

	vertexNormals.resize(vertexCount);
	vertexNormalsCount.resize(vertexCount);
	for (int i = 0; i < triangleCount; i++) {
		Triangle triangle = GetTriangleData(indices, i);
		glm::vec3 a = GetVertexData(vertices, triangle.a);
		glm::vec3 b = GetVertexData(vertices, triangle.b);
		triangleNormals.push_back(glm::normalize(glm::cross(a, b)));
	}

	for (int i = 0; i < triangleNormals.size(); i++) {
		Triangle triangle = GetTriangleData(indices, i);
		//std::cout << i << " " << indices[i * 3] << " " << indices[(i * 3) + 1] << " " << indices[(i * 3) + 2] << std::endl;
		vertexNormals[triangle.a] += triangleNormals[i];
		vertexNormalsCount[triangle.a]++;
		vertexNormals[triangle.b] += triangleNormals[i];
		vertexNormalsCount[triangle.b]++;
		vertexNormals[triangle.c] += triangleNormals[i];
		vertexNormalsCount[triangle.c]++;
	}

	for (int i = 0; i < vertexNormals.size(); i++) {
		vertexNormals[i] /= vertexNormalsCount[i];
		SetNormalData(vertices, vertexNormals[i], i);
	}

}

Triangle SceneLoader::GetTriangleData(GLuint* indices, int index) {
	Triangle triangle;
	triangle.a = indices[(index * 3)];
	triangle.b = indices[(index * 3) + 1];
	triangle.c = indices[(index * 3) + 2];
	return triangle;
}

glm::vec3 SceneLoader::GetVertexData(GLfloat* vertices, int index) {
	glm::vec3 vertex;
	vertex.x = vertices[(index * 8)];
	vertex.y = vertices[(index * 8) + 1];
	vertex.z = vertices[(index * 8) + 2];
	return vertex;
}

void SceneLoader::SetNormalData(GLfloat* vertices, glm::vec3 normal, int index) {
	vertices[(index * 8) + 3] = normal.x;
	vertices[(index * 8) + 4] = normal.y;
	vertices[(index * 8) + 5] = normal.z;
}

void SceneLoader::LoadBakedData(std::vector<Object>* objects, std::ifstream* file) {
	std::string line;
	while (std::getline(*file, line)) {
		std::stringstream ss(line);
		int id;
		float x, y, z;
		ss >> id >> x >> y >> z;
		(*objects)[id].bakedPositions.push_back(glm::vec3(x, y, z));
	}
}

