#pragma once

#include "Scene.h"

class MainScene : public Scene {

public:

	MainScene();

	virtual void InitializeScene() override;
	virtual void OnSceneSwitch() override;
	virtual void Update(float dt) override;
	virtual void Render() override;

private:

	void OpenSimulationFile();

	void OpenBakedFile();

};