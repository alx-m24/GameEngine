#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Inputs and outputs
namespace IO {
	unsigned int SCR_WIDTH = 1000;
	unsigned int SCR_HEIGHT = 600;

	bool lastF4;
	bool firstMouse = true;
	bool useCam = true;

	float lastX = SCR_WIDTH / 2.0f;
	float lastY = SCR_HEIGHT / 2.0f;
	float xoffset = 0.0f;
	float yoffset = 0.0f;
	float yScrollOffset = 0.0f;

	bool wireFrame = false;
}

using namespace IO;

void inline framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

	SCR_WIDTH = width;
	SCR_HEIGHT = height;
}

void inline mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	xoffset = xpos - lastX;
	yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;
}

void inline scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	yScrollOffset = static_cast<float>(yoffset);
}

void inline processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		IO::useCam = false;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1)) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		IO::useCam = true;
	}

	bool F4 = glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS;
	if (F4 && !lastF4) {
		wireFrame = !wireFrame;

		if (wireFrame) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	lastF4 = F4;

	yScrollOffset = 0.0f;
	xoffset = 0.0f;
	yoffset = 0.0f;
}
