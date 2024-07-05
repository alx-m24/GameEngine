#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Resources/Resources.hpp"

using namespace Resources;

void setupBuffers() {
	buffers["cubeVAO"] = 0;
	buffers["cubeVBO"] = 0;
	unsigned int& cubeVAO = buffers["cubeVAO"];
	unsigned int& cubeVBO = buffers["cubeVBO"];

	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);

	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices::cube), Vertices::cube, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	buffers["lightVAO"] = 0;
	unsigned int& lightVAO = buffers["lightVAO"];
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, buffers["cubeVBO"]);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void loadShaders() {
	shaders["lightCube"] = Shader(PATH + "\\Shaders\\lightCube.vert", PATH + "\\Shaders\\lightCube.frag");
	shaders["lighting"] = Shader(PATH + "\\Shaders\\LightEnvironment.vert", PATH + "\\Shaders\\LightEnvironment.frag");
}

void loadTextures() {
	textures["diffuseMap"] = loadTexture(PATH + "\\Textures\\container2.png");
	textures["specularMap"] = loadTexture(PATH + "\\Textures\\container2_specular.png");
}

void loadModels() {
	models["BackBag"] = Model(PATH + "\\Models\\BackBag\\backpack.obj");
	models["Sponza"] = Model(PATH + "\\Models\\Sponza-master\\sponza.obj");
}

void cleanup() {
	glDeleteVertexArrays(1, &buffers["cubeVAO"]);
	glDeleteVertexArrays(1, &buffers["lightVAO"]);
	glDeleteBuffers(1, &buffers["cubeVBO"]);
}