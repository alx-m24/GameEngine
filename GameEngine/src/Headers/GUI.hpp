#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include "Resources/Resources.hpp"
#include "LightingSystem.hpp"

using namespace Resources;

class GUI {
private:
	unsigned int frameNum = 0, framerate = 0;
	float lastFrame = 0.0;

	LightingSystem& lightSys;

private:
	void metricsWindow();
	void lightWindow();

public:
	GUI(GLFWwindow* window, LightingSystem& lightSys);

	void update(float time);
	void draw();
};

