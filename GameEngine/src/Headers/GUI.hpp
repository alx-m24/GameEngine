#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include "Resources/Resources.hpp"

using namespace Resources;

class GUI {
private:
	bool show_demo_window = true;
	bool show_another_window = true;

public:
	GUI(GLFWwindow* window);

	void update();
};

