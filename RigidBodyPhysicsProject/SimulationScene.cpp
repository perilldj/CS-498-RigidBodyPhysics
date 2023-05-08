#include "SimulationScene.h"
#include <locale>
#include <codecvt>

void SimulationScene::InitializeScene() {

}

void SimulationScene::OnSceneSwitch() {
	doPhysicsOnUpdate = false;
}

void SimulationScene::Update(float dt) {
	Scene::Update(dt);
}

void SimulationScene::Render() {

	ImGui::Begin("CS-498 Capstone: Simulation");

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	if (ImGui::Button("Start Simulation"))
		doPhysicsOnUpdate = true;
	if (ImGui::Button("Stop Simulation")) {
		doPhysicsOnUpdate = false;
		if (isBaking) {
			SaveBake();
			isBaking = false;
		}
	}

	if (ImGui::Button("Reset Simulation")) {
		doPhysicsOnUpdate = false;
		for (int i = 0; i < objects.size(); i++) {
			objects[i].position = objects[i].originalPosition;
			objects[i].rotation = glm::vec3(0.0f, 0.0f, 0.0f);
			objects[i].velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		}
	}

	ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "\nPrecompute computationally\nheavy simulation to a file (.bake)\n");
	if(isBaking)
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "\nCurrently Baking Simulation.\nPress the 'Stop Simulation'\nbutton when you are finished baking.\n");

	if (ImGui::Button("Bake Simulation")) {
		doPhysicsOnUpdate = true;
		for (int i = 0; i < objects.size(); i++) {
			objects[i].position = objects[i].originalPosition;
			objects[i].rotation = glm::vec3(0.0f, 0.0f, 0.0f);
			objects[i].velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		}
		isBaking = true;
	}

	if(isBaking)
		BakePositions();

	ImGui::Text("\nApplication average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::End();

	Scene::Render();

}

void SimulationScene::BakePositions() {
	for (int i = 0; i < objects.size(); i++) {
		objects[i].bakedPositions.push_back(objects[i].position);
	}
}

void SimulationScene::SaveBake() {
	
	std::wstring filepath(szFile);
	std::wstring newExtension = L".bake";
	size_t dot_pos = filepath.find_last_of('.');
	if (dot_pos != std::string::npos) {
		filepath.replace(dot_pos, filepath.length() - dot_pos, newExtension);
	} else {
		filepath += newExtension;
	}

	std::wofstream bakedFile(filepath);

	if (!bakedFile.is_open())
		return;

	for (int i = 0; i < strFile.size(); i++) {
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		std::wstring wstr = converter.from_bytes(strFile[i]);
		bakedFile << wstr << std::endl;
	}

	bakedFile << L"!" << std::endl;

	for (int i = 0; i < objects.size(); i++) {
		for (int j = 0; j < objects[i].bakedPositions.size(); j++) {
			bakedFile << i << L" " << objects[i].bakedPositions[j].x << L" " << objects[i].bakedPositions[j].y << L" " << objects[i].bakedPositions[j].z << std::endl;
		}
	}

	bakedFile.close();

}

void SimulationScene::LoadSimulation() {
	if (szFile != nullptr)
		SceneLoader::LoadScene(szFile, &objects, defaultShader, strFile);
}
