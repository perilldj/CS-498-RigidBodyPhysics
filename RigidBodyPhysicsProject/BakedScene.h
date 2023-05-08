#pragma once

#pragma once

#include "SceneLoader.h"
#include "Scene.h"

class BakedScene : public Scene {

public:

	wchar_t* szFile;
	int szFileSize = 260;

	std::vector<std::string> strFile;

	int tick = 0;
	bool play = false;

	virtual void InitializeScene() override;
	virtual void OnSceneSwitch() override;
	virtual void Update(float dt) override;
	virtual void Render() override;

	void LoadSimulation();

};