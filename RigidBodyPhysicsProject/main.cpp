#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <chrono>
#include <thread>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Object.h"
#include "Camera.h"
#include "Texture.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include "Scene.h"
#include "MainScene.h"
#include "SimulationScene.h"
#include "BakedScene.h"

#include "main.h"

unsigned int width = 1920;
unsigned int height = 1080;
Camera camera(width, height, glm::vec3(0.0f, 0.0f, 0.1f));

MainScene mainScene = MainScene();
SimulationScene simulationScene = SimulationScene();
BakedScene bakedScene = BakedScene();
Scene* currentScene = &mainScene;

/*

	CS-498 Rigid Body Physics Capstone
	Written By: Dean Perillo

*/

/*
	Initial 3D OpenGL tutorial to get this project off the ground: https://www.youtube.com/watch?v=45MIykWJ-C4
*/

int main() {

	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a glfw window, check if the window was successfully created.
	// If it failed to create, terminate the program
	GLFWwindow* window = glfwCreateWindow(width, height, "CS-498 Rigid Body Physics Capstone", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window..." << std::endl;
		glfwTerminate();
		return -1;
	}

	// Use the created window
	glfwMakeContextCurrent(window);

	// Load GLAD so it configures OpenGL
	gladLoadGL();

	// Specify the viewport size of OpenGL in the window
	glViewport(0, 0, width, height);

	// Create and compile a shader
	Shader shaderProgram("default.vert", "default.frag");

	// Create a texture
	Texture texture("roofing.png", GL_TEXTURE_2D, GL_TEXTURE, GL_RGBA, GL_UNSIGNED_BYTE);
	// Set the texture slot
	texture.texUnit(shaderProgram, "tex0", 0);

	glEnable(GL_DEPTH_TEST);

	glfwSetCursorPosCallback(window, MouseCallback);

	const char* glsl_version = "#version 330 core";
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGui::StyleColorsDark();

	mainScene.defaultShader = &shaderProgram;
	simulationScene.defaultShader = &shaderProgram;
	bakedScene.defaultShader = &shaderProgram;

	bakedScene.InitializeScene();
	mainScene.InitializeScene();
	currentScene->InitializeScene();

	std::chrono::high_resolution_clock::time_point prevTime = std::chrono::high_resolution_clock::now();;
	std::chrono::high_resolution_clock::time_point currentTime;
	double deltaTime;

	// If for any reason the window needs to close, exit the loop
	while (!glfwWindowShouldClose(window)) {
		
		currentTime = std::chrono::high_resolution_clock::now();
		deltaTime = std::chrono::duration_cast<std::chrono::duration<double>>(currentTime - prevTime).count();
		prevTime = currentTime;

		// Set clear color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Set clear buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Use shader program
		shaderProgram.Activate();
		
		camera.Inputs(window);
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

		texture.Bind();

		currentScene->Update(1.0f / 200.0f);
		currentScene->Render();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Display the new buffer
		glfwSwapBuffers(window);

		// Process events
		glfwPollEvents();

		std::chrono::milliseconds frameTime(static_cast<int>(0.005));
		std::this_thread::sleep_for(frameTime);

	}

	currentScene->Cleanup();

	// Delete objects
	texture.Delete();
	shaderProgram.Delete();

	// Terminate program
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	return 0;

}

static void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	if(!io.WantCaptureMouse)
		camera.MouseCallback(window, xpos, ypos);
}

void SwitchSimulation(Scene* scene) {
	if (currentScene != nullptr)
		currentScene->Cleanup();
	currentScene = scene;
	scene->OnSceneSwitch();
}

void OpenSimulationScene(wchar_t szFile[260]) {
	wchar_t* newSZFile = new wchar_t[260];
	for (int i = 0; i < 260; i++)
		newSZFile[i] = szFile[i];
	SwitchSimulation(&simulationScene);
	simulationScene.szFile = newSZFile;
	simulationScene.LoadSimulation();
}

void OpenBakedScene(wchar_t szFile[260]) {
	wchar_t* newSZFile = new wchar_t[260];
	for (int i = 0; i < 260; i++)
		newSZFile[i] = szFile[i];
	SwitchSimulation(&bakedScene);
	bakedScene.szFile = newSZFile;
	bakedScene.LoadSimulation();
}