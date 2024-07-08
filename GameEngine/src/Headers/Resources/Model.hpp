#pragma once
#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>
#include <unordered_map>
#include "Mesh.hpp"
#include "Shaders/Shader.hpp"

struct Transformations {
	glm::vec3 position = { 0.0f, 0.0f, 0.0f };
	glm::vec4 rotation = { 1.0f, 1.0f, 1.0f, 0.0f };
	glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
};

struct Instanced {
	unsigned int VBO = 0;
	std::vector<glm::mat4> models;
	std::vector<Transformations> objects;
};


glm::mat4 getModelMatrix(Transformations& transformation);

class Model {
public:
	Model();
	Model(std::string path);

public:
	std::vector<Mesh> meshes;
	Instanced instances;

public:
	void update();
	void draw(Shader& shader);

private:
	unsigned int instanceVBO = 0;
	std::vector<Texture> textures_loaded;
	std::string directory;

private:
	void setupVAOs();
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

#endif // !MODEL_H
