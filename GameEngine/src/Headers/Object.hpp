#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Resources/Model.hpp" // Always include this Header as it is the implementation of "Transformations" & "Instances"
#include "Resources/Resources.hpp"

using namespace Resources;

class Containers {
private:
	Instanced instances;

public:
	Containers(glm::vec3 positions[], unsigned int count);

	void update(float time);
	void draw(Shader& shader);
};

class LightCubes {
public:
	Instanced instances;
	unsigned int instancedColorsVBO = 0;
	std::vector<glm::vec3> colors;

public:
	LightCubes() = default;
	LightCubes(glm::vec3 positions[], glm::vec3 colors[], unsigned int count);

	void update();
	void draw(Shader& shader);
};