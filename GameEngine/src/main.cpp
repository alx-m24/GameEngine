// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// Others
#include <iostream>
#include <filesystem>
// My headers
#include "Headers/Resources/Textures/Textures.hpp"
#include "Headers/Resources/Shaders/Shader.hpp"
#include "Headers/Resources/Resources.hpp"
#include "Headers/Resources/Model.hpp"
#include "Headers/LightingSystem.hpp"
#include "Headers/Camera/Camera.hpp"
#include "Headers/IO/Input.hpp"
#include "Headers/Buffers.hpp"
#include "Headers/Object.hpp"
#include "Headers/GUI.hpp"

using namespace IO;
using namespace Resources;

int main() {
#pragma region init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#pragma endregion

#pragma region Window and Context
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Lighting", NULL, NULL);
	if (window == nullptr) {
		std::cerr << "Failed to create window" << std::endl;
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
#pragma endregion
	
#pragma region OpenGL Parameters
	stbi_set_flip_vertically_on_load(true);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
#pragma endregion

#pragma region Load Resources
	loadShaders();
	loadTextures();
	loadModels();
	setupBuffers();
#pragma endregion

#pragma region Object Parameters
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	glm::vec3 lightColor[] = {
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	};;

	float time = static_cast<float>(glfwGetTime());
	float lastTime = time;
#pragma endregion

#pragma region Light
	LightingSystem lightSys(pointLightPositions, lightColor, 4);

	lightSys.dirLights.emplace_back(
		DirectionalLight{
			glm::vec3(-0.2f, -1.0f, -0.3f),
		});

	for (int i = 0; i < 4; ++i) {
		lightSys.pointLights.emplace_back(
			PointLight{
				pointLightPositions[i]
			});
		lightSys.pointLights[i].color = lightColor[i];
	}

	lightSys.SpotLights.emplace_back(
		SpotLight{
			glm::vec3(0.0f, 10.0f, 0.0f)
		});
#pragma endregion

#pragma region Objects
	Containers containers(cubePositions, 10);

	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

	models["BackBag"].instances.objects.emplace_back(Transformations{ {5.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 0.0f}, {0.5f, 0.5f, 0.5f} });
	models["BackBag"].instances.objects.emplace_back(Transformations{ {0.0f, 0.0f, 5.0f}, {1.0f, 1.0f, 1.0f, 0.0f}, {0.5f, 0.5f, 0.5f} });
	models["Sponza"].instances.objects.emplace_back(Transformations{ {0.0f, 0.0f, -10.0f}, {1.0f, 1.0f, 1.0f, 0.0f}, {0.02f, 0.02f, 0.02f} });

	GUI gui(
		window,
		lightSys
	);
#pragma endregion
	while (!glfwWindowShouldClose(window)) {
#pragma region Time
		time = static_cast<float>(glfwGetTime());
		float dt = time - lastTime;
		lastTime = time;
#pragma endregion

#pragma region Inputs
		glfwPollEvents();
		UpdateParameters cameraUpdateParams = {
			xoffset,
			yoffset,
			yScrollOffset
		};
		if (useCam) camera.update(window,cameraUpdateParams, dt);
		processInput(window);
#pragma endregion

#pragma region Update
		if (!useCam) gui.update(time);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		glBindBuffer(GL_UNIFORM_BUFFER, buffers["matricesUBO"]);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		shaders["lighting"].use();
		shaders["lighting"].setVec3("viewPos", camera.Position);

		lightSys.update(shaders["lighting"]);
		containers.update(time);

		models["BackBag"].instances.objects[0].rotation = {1.5f, 1.0f, 0.0f, time * 50.0f};
		models["BackBag"].update();
		models["Sponza"].update();
#pragma endregion

#pragma region Draw
		containers.draw(shaders["lighting"]);
		
		models["BackBag"].draw(shaders["lighting"]);
		models["Sponza"].draw(shaders["lighting"]);

		shaders["lightCube"].use();
		lightSys.drawLightSources(shaders["lightCube"]);

		gui.draw();

		glfwSwapBuffers(window);
#pragma endregion
	}
	cleanup();

	glfwTerminate();

	return EXIT_SUCCESS;
}