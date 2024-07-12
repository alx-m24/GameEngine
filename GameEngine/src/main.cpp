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

void debugQuad();

using namespace IO;
using namespace Resources;

int main() {
#pragma region init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
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

	glEnable(GL_MULTISAMPLE);
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
	bool gamma = true;
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
	containers.instances.shininess = 32.0f;
	containers.instances.objects[0] = {
		{0.0f, -5.0f, 0.0f},
		{1.0f, 1.0f, 1.0f, 0.0f},
		{25.0f, 1.0f, 25.0f}
	};

	unsigned int usedCamIdx = 0;
	std::vector<Camera> cameras = { 
		Camera(glm::vec3(0.0f, 0.0f, 3.0f)),
		Camera(glm::vec3(5.0f, 10.0f, 3.0f)),
		Camera(glm::vec3(-5.0f, -10.0f, 0.0f)),
		Camera(glm::vec3(0.0f, 25.0f, -5.0f))
	};

	models["BackBag"].instances.objects.emplace_back(Transformations{ {5.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 0.0f}, {0.5f, 0.5f, 0.5f} });
	models["BackBag"].instances.objects.emplace_back(Transformations{ {0.0f, 0.0f, 5.0f}, {1.0f, 1.0f, 1.0f, 0.0f}, {0.5f, 0.5f, 0.5f} });
	models["Sponza"].instances.objects.emplace_back(Transformations{ {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 0.0f}, {0.02f, 0.02f, 0.02f} });

	models["BackBag"].instances.shininess = 12.0f;
	models["Sponza"].instances.shininess = 64.0f;

	std::vector<std::pair<std::string, std::vector<Transformations>&>> objects;
	objects.push_back({ "Containers", containers.instances.objects});
	objects.push_back({ "BackBags", models["BackBag"].instances.objects });
	objects.push_back({ "Sponza", models["Sponza"].instances.objects});

	GUI gui(
		window,
		lightSys,
		usedCamIdx,
		cameras,
		objects
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
		if (useCam) cameras[usedCamIdx].update(window, cameraUpdateParams, dt);
		processInput(window);
#pragma endregion

#pragma region Update
		gui.update(time);

		glm::mat4 view = cameras[usedCamIdx].GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(cameras[usedCamIdx].Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		glBindBuffer(GL_UNIFORM_BUFFER, buffers["matricesUBO"]);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		shaders["lighting"].use();
		shaders["lighting"].setBool("gamma", gamma);
		shaders["lighting"].setVec3("viewPos", cameras[usedCamIdx].Position);

		lightSys.update(shaders["lighting"]);
		containers.update(time);

		models["BackBag"].instances.objects[0].rotation = {1.5f, 1.0f, 0.0f, time * 50.0f};
		models["BackBag"].update();
		models["Sponza"].update();
#pragma endregion

#pragma region Draw
		//glEnable(GL_CULL_FACE);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#pragma region First Pass (depth map)
		glCullFace(GL_FRONT);
		// For the moment, only 1 directional light can have shadows
		// Use ORTHO for directional and PERSPECTIVE for point

		const float near = -50.0f, far = 50.0f, factor = 30.0f;

		glm::mat4 lightProjection = glm::ortho(-factor, factor, -factor, factor, near, far);
		glm::mat4 lightView = glm::lookAt(
			lightSys.dirLights[0].direction * -1.0f, // Position
			glm::vec3(0.0f, 0.0f, 0.0f), // Origin (Scene center)
			glm::vec3(0.0f, 1.0f, 0.0f)); // Up vector
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;

		shaders["depth"].use();
		shaders["depth"].setMat4("lightSpaceMatrix", lightSpaceMatrix);

		glViewport(0, 0, ShadowMapResolution, ShadowMapResolution);
		glBindFramebuffer(GL_FRAMEBUFFER, buffers["depthMapFBO"]);
		glClear(GL_DEPTH_BUFFER_BIT);

		containers.draw(shaders["depth"]);
		models["BackBag"].draw(shaders["depth"]);
		models["Sponza"].draw(shaders["depth"]);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glCullFace(GL_BACK);
#pragma endregion
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#pragma region Second Pass (final pass)		
		
		shaders["lighting"].use();
		shaders["lighting"].setMat4("lightSpaceMatrix", lightSpaceMatrix);
		shaders["lighting"].setInt("shadowMap", 3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, textures["depthMap"]);

		containers.draw(shaders["lighting"]);
		
		models["BackBag"].draw(shaders["lighting"]);
		models["Sponza"].draw(shaders["lighting"]);

		shaders["lightCube"].use();
		lightSys.drawLightSources(shaders["lightCube"]);
		/*
		glDisable(GL_CULL_FACE);
		shaders["debug"].use();
		shaders["debug"].setInt("depthMap", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures["depthMap"]);
		debugQuad();*/

		gui.draw();

		glfwSwapBuffers(window);
#pragma endregion
#pragma endregion
	}
	cleanup();

	glfwTerminate();

	return EXIT_SUCCESS;
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void debugQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}