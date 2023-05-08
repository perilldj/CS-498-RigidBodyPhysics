#include "Scene.h"


Scene::Scene() {

}

void Scene::InitializeScene() {
	/*
	AddDefaultObject(DefaultObjectType::Cube, glm::vec3(0.0f, 0.2f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), defaultShader);
	AddDefaultObject(DefaultObjectType::Cube, glm::vec3(0.0f, 0.2f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), defaultShader);
	AddDefaultObject(DefaultObjectType::Cube, glm::vec3(1.0f, 0.7f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), defaultShader);
	AddDefaultObject(DefaultObjectType::Cube, glm::vec3(-1.0f, 2.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), defaultShader);
	AddDefaultObject(DefaultObjectType::Cube, glm::vec3(0.0f, 1.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), defaultShader);
	AddDefaultObject(DefaultObjectType::Cube, glm::vec3(0.0f, 1.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), defaultShader);
	*/
}

void Scene::OnSceneSwitch() {

}

void Scene::Update(float dt) {
	if (!doPhysicsOnUpdate)
		return;
	for (int i = 0; i < objects.size(); i++) 
		objects[i].Integrate(dt);
	for (int i = 0; i < objects.size(); i++)
		objects[i].Resolve(&objects, dt);
}

void Scene::Render() {

	/*
	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
	ImGui::Checkbox("Another Window", &show_another_window);

	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	//io.WantCaptureMouse = true;
	ImGui::End();
	*/

	for (int i = 0; i < objects.size(); i++)
		objects[i].draw();
}

int Scene::AddObject(glm::vec3 position, glm::vec3 rotation, GLfloat* meshData, int meshSize, GLuint* indicesData, int indicesSize, Shader* shader) {
	objects.push_back(Object(meshData, meshSize, indicesData, indicesSize, shader));
	int objectIndex = objects.size() - 1;
	objects[objectIndex].position = position;
	objects[objectIndex].rotation = rotation;
	return objects.size() - 1;
}

int Scene::AddDefaultObject(DefaultObjectType objectType, glm::vec3 position, glm::vec3 rotation, Shader* shader) {
	switch (objectType) {
	case DefaultObjectType::Pyramid:
		return AddObject(position, rotation, Object::pyramidMeshVertices, Object::pyramidMeshVerticesSize, Object::pyramidMeshIndices, Object::pyramidMeshIndicesSize, shader);
	}
}

void Scene::Cleanup() {
	for (int i = 0; i < objects.size(); i++)
		objects[i].destroy();
}
