#include "BakedScene.h"
#include <locale>
#include <codecvt>

void BakedScene::InitializeScene() {

}

void BakedScene::OnSceneSwitch() {
	doPhysicsOnUpdate = false;
}

void BakedScene::Update(float dt) {
	Scene::Update(dt);
}

void BakedScene::Render() {

	ImGui::Begin("CS-498 Capstone: Baked");

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	if (ImGui::Button("Start Simulation"))
		play = true;
	if (ImGui::Button("Stop Simulation")) {
		play = false;
	}

	if (ImGui::Button("Reset Simulation")) {
		play = false;
		tick = 0;
		for (int i = 0; i < objects.size(); i++) {
			objects[i].position = objects[i].originalPosition;
			objects[i].rotation = glm::vec3(0.0f, 0.0f, 0.0f);
			objects[i].velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		}
	}

	ImGui::Text("\nApplication average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::End();

	if (play) {

		tick++;

		for (int i = 0; i < objects.size(); i++) {
			if (tick < objects[i].bakedPositions.size() - 1)
				objects[i].position = objects[i].bakedPositions[tick];
		}

	}


	Scene::Render();

}

void BakedScene::LoadSimulation() {
	if (szFile != nullptr)
		SceneLoader::LoadScene(szFile, &objects, defaultShader, strFile);
}
