#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include "Resources/Resources.hpp"
#include "LightingSystem.hpp"
#include "Camera/Camera.hpp"

using namespace Resources;

class GUI {
private:
	unsigned int frameNum = 0, framerate = 0;
	float lastFrame = 0.0;
	bool lightingWindow = true;

	LightingSystem& lightSys;

private:
	unsigned int& usedCamIdx;
	std::vector<Camera>& cameras;
	// Reference to a std::Vector that contains std::Pairs<std::string, std::Vector of Transforms>
	std::vector<std::pair<std::string, std::vector<Transformations>&>>& objects;

private:
	void lightAndCamWindow();
	void lighting();
	void cameraWindow();
	void objectsWindow();

public:
	GUI(GLFWwindow* window, LightingSystem& lightSys, unsigned int& usedCamIdx, std::vector<Camera>& cameras, std::vector<std::pair<std::string, std::vector<Transformations>&>>& objects);

	void update(float time);
	void draw();
};

