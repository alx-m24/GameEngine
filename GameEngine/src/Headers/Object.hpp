#pragma once
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// Other
#include <vector>
#include "Resources/Shaders/Shader.hpp"

struct Transformations {
	glm::vec3 position = { 0.0f, 0.0f, 0.0f };
	// Last term is the angle in degrees
	glm::vec4 rotation = { 1.0f, 1.0f, 1.0f, 0.0f };
	glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
	glm::vec3 color = { 1.0f, 1.0f, 1.0f };
};

class Objects : public std::vector<Transformations> {
private:
	unsigned int VAO;
	unsigned int vertexNum;

public:
	unsigned int diffuse = 0, specular = 0;
	float shininess = 64.0f;
	bool useColor = false;

public:
	Objects(unsigned int VAO, unsigned int vertexNum) : VAO(VAO), vertexNum(vertexNum) {};

	void draw(Shader& shader);
};