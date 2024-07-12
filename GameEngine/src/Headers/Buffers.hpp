#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Resources/Resources.hpp"
#include <iostream>

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

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * 36, Vertices::cube, GL_STATIC_DRAW);

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

	buffers["matricesUBO"] = 0;
	unsigned int& matricesUBO = buffers["matricesUBO"];
	glGenBuffers(1, &matricesUBO);

	glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, matricesUBO, 0, 2 * sizeof(glm::mat4));

	unsigned int matrices_index = glGetUniformBlockIndex(shaders["lighting"].ID, "matrices");
	glUniformBlockBinding(shaders["lighting"].ID, matrices_index, 0);

	matrices_index = glGetUniformBlockIndex(shaders["lightCube"].ID, "matrices");
	glUniformBlockBinding(shaders["lightCube"].ID, matrices_index, 0);

	buffers["depthMapFBO"] = 0;
	unsigned int& depthMapFBO = buffers["depthMapFBO"];
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	textures["depthMap"] = 0;
	unsigned int& depthMap = textures["depthMap"];
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void loadShaders() {
	shaders["lightCube"] = Shader(PATH + "\\Shaders\\lightCube.vert", PATH + "\\Shaders\\lightCube.frag");
	shaders["lighting"] = Shader(PATH + "\\Shaders\\LightEnvironment.vert", PATH + "\\Shaders\\LightEnvironment.frag");
	shaders["depth"] = Shader(PATH + "\\Shaders\\depthShader.vert", PATH + "\\Shaders\\depthShader.frag");
	shaders["debug"] = Shader(PATH + "\\Shaders\\debug.vert", PATH + "\\Shaders\\debug.frag");
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