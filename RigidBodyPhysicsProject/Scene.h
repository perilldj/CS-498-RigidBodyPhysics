#pragma once

#include <vector>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include "Object.h"

enum DefaultObjectType {
	Pyramid
};

/* 
	Class: Scene
	Description: Parent scene class, child objects inheriting this can act as various menus in the simulation.
			     Inherited in MainScene, SimulationScene, and BakedScene
*/

class Scene {

public:

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	Shader* defaultShader;

	bool doPhysicsOnUpdate = true;

	std::vector<Object> objects;

	Scene();

	virtual void InitializeScene();
	virtual void OnSceneSwitch();
	virtual void Update(float dt);
	virtual void Render();

	int AddObject(glm::vec3 position, glm::vec3 rotation, GLfloat* meshData, 
				  int meshSize, GLuint* indicesData, int indicesSize, Shader* shader);
	int AddDefaultObject(DefaultObjectType objectType, glm::vec3 position, 
					     glm::vec3 rotation, Shader* shader);

	virtual void Cleanup();

};