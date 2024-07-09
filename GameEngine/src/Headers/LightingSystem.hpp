#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "Resources/Shaders/Shader.hpp"
#include "Resources/Resources.hpp"
#include "Object.hpp"

using namespace Resources;

struct DirectionalLight {
	glm::vec3 direction = { 0.0f, 0.0f, -1.0f };
	glm::vec3 ambient = { 0.14f, 0.14f, 0.14f };
	glm::vec3 diffuse = { 0.58f, 0.58f, 0.58f };
	glm::vec3 specular = { 1.0f, 1.0f, 1.0f };
	glm::vec3 color = { 1.0f, 1.0f, 1.0f };
};

struct PointLight {
	glm::vec3 position = { 0.0f, 0.0f, 0.0f };
	glm::vec3 ambient = { 0.05f, 0.05f, 0.05f };
	glm::vec3 diffuse = { 0.8f, 0.8f, 0.8f };
	glm::vec3 specular = { 1.0f, 1.0f, 1.0f };
	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;
	glm::vec3 color =  { 1.0f, 1.0f, 1.0f };
};

struct SpotLight {
	glm::vec3 position = { 0.0f, 0.0f, 0.0f };
	glm::vec3 direcion = { 0.0f, -1.0f, 0.0f };
	glm::vec3 ambient = { 0.0f, 0.0f, 0.0f };
	glm::vec3 diffuse = { 1.0f, 1.0f, 1.0f };
	glm::vec3 specular = { 1.0f, 1.0f, 1.0f };
	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;
	float cutOff = glm::cos(glm::radians(12.5f));
	float outerCutOff = glm::cos(glm::radians(15.0f));
	glm::vec3 color = { 1.0f, 1.0f, 1.0f };
};

class LightingSystem {
public:
	std::vector<DirectionalLight> dirLights;
	std::vector<PointLight> pointLights;
	std::vector<SpotLight> SpotLights;
	LightCubes lightCubes;

public:
	LightingSystem(glm::vec3 pointPos[], glm::vec3 pointColor[], unsigned int count);

	void update(Shader& shader);
	void drawLightSources(Shader& shader);
	void addDirectionalLight(DirectionalLight dirLight);
	void addPointLight(PointLight pointlight);
	void addSpotlLight(SpotLight spotLight);
};

