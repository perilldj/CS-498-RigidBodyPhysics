#pragma once

#include "SceneLoader.h"
#include "Scene.h"

class SimulationScene : public Scene {

public:

	wchar_t* szFile;
	int szFileSize = 260;

	bool isBaking = false;

	std::vector<std::string> strFile;

	virtual void InitializeScene() override;
	virtual void OnSceneSwitch() override;
	virtual void Update(float dt) override;
	virtual void Render() override;

	void BakePositions();
	void SaveBake();

	void LoadSimulation();

};