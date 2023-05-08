#include "MainScene.h"
#include "main.h"
#include <windows.h>
#include <commdlg.h>

MainScene::MainScene() {
	doPhysicsOnUpdate = false;
}

void MainScene::InitializeScene() {
	Scene::InitializeScene();
}

void MainScene::OnSceneSwitch() {
	Scene::OnSceneSwitch();
}

void MainScene::Update(float dt) {
	Scene::Update(dt);
}

void MainScene::Render() {

	ImGui::Begin("CS-498 Capstone: Main Menu");

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::Text("Rigid Body Physics Simulator"); 
	ImGui::Text("Written by Dean Perillo");

	if (ImGui::Button("Open Simulation Scene (*.obj)"))
		OpenSimulationFile();
	if (ImGui::Button("Open Baked Simulation (*.bake)"))
		OpenBakedFile();

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::End();

	Scene::Render();

}

void MainScene::OpenSimulationFile() {
	
	// File explorer code modified from official microsoft documentation
	// https://learn.microsoft.com/en-us/windows/win32/dlgbox/using-common-dialog-boxes

	OPENFILENAME ofn;       // common dialog box structure
	wchar_t szFile[260];       // buffer for file name
	HWND hwnd = HWND();              // owner window
	HANDLE hf;              // file handle

	LPCWSTR filter = L"Object File\0*.obj*\0";

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 

	GetOpenFileName(&ofn);

	OpenSimulationScene(szFile);

}

void MainScene::OpenBakedFile() {

	// File explorer code modified from official microsoft documentation
	// https://learn.microsoft.com/en-us/windows/win32/dlgbox/using-common-dialog-boxes

	OPENFILENAME ofn;       // common dialog box structure
	wchar_t szFile[260];       // buffer for file name
	HWND hwnd = HWND();              // owner window
	HANDLE hf;              // file handle

	LPCWSTR filter = L"Baked Simulation\0*.bake*\0";

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 

	GetOpenFileName(&ofn);

	OpenBakedScene(szFile);

}
