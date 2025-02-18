#include "Object.hpp"

Containers::Containers(glm::vec3 positions[], unsigned int count)
{
	for (unsigned int i = 0; i < count; ++i) {
		instances.objects.emplace_back(Transformations{ positions[i] });
		instances.models.emplace_back(getModelMatrix(instances.objects[i]));
	}

	glBindVertexArray(buffers["cubeVAO"]);

	glGenBuffers(1, &instances.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, instances.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * std::max(1u, count), nullptr, GL_DYNAMIC_DRAW);

	size_t vec4Size = sizeof(glm::vec4);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);
}

void Containers::update(float time)
{
	unsigned int n = instances.objects.size();
	if (n != instances.models.size()) instances.models.resize(n);

	for (unsigned int i = 0; i < n; ++i) {
		instances.objects[i].rotation.w = time * i * 10;
		instances.models[i] = getModelMatrix(instances.objects[i]);
	}

	glBindBuffer(GL_ARRAY_BUFFER, instances.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * n, &instances.models[0], GL_DYNAMIC_DRAW);
}

void Containers::draw(Shader& shader)
{
	shader.setFloat("material.shininess", instances.shininess);

	shader.setInt("material.diffuse", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures["diffuseMap"]);
	shader.setInt("material.specular", 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures["specularMap"]);

	glBindVertexArray(buffers["cubeVAO"]);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 36, instances.objects.size());
	glBindVertexArray(0);
}

void Containers::add(glm::vec3 position)
{
	instances.objects.emplace_back(Transformations{ position });
}

void Containers::erase(unsigned int idx)
{
	instances.objects.erase(instances.objects.begin() + idx);
}

void Containers::resize(size_t size)
{
	instances.objects.reserve(size);
}

size_t Containers::getSize()
{
	return size_t(instances.objects.size());
}

Transformations& Containers::operator[](unsigned int idx)
{
	return instances.objects[idx];
}

LightCubes::LightCubes(glm::vec3 positions[], glm::vec3 colors[], unsigned int count)
{
	for (unsigned int i = 0; i < count; ++i) {
		instances.objects.emplace_back(Transformations{ positions[i], {1.0f, 1.0f, 1.0f, 0.0f}, {0.5f,0.5f,0.5f} });
		instances.models.emplace_back(getModelMatrix(instances.objects[i]));
		this->colors.emplace_back(colors[i]);
	}

	glGenBuffers(1, &instancedColorsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instancedColorsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * std::max(1u, count), nullptr, GL_DYNAMIC_DRAW);

	glBindVertexArray(buffers["lightVAO"]);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glVertexAttribDivisor(1, 1);

	unsigned int& instancedVBO = instances.VBO;
	glGenBuffers(1, &instancedVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instancedVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * count, &instances.models[0], GL_DYNAMIC_DRAW);

	std::size_t vec4Size = sizeof(glm::vec4);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);

	glBindVertexArray(0);
}

void LightCubes::update()
{
	unsigned int n = instances.objects.size();
	if (n != instances.models.size()) instances.models.resize(n);

	for (unsigned int i = 0; i < n; ++i) {
		instances.models[i] = getModelMatrix(instances.objects[i]);
	}

	glBindBuffer(GL_ARRAY_BUFFER, instancedColorsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * n, &colors[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, instances.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * n, &instances.models[0], GL_DYNAMIC_DRAW);
}

void LightCubes::draw(Shader& shader)
{
	shader.setFloat("material.shininess", instances.shininess);

	glBindVertexArray(buffers["lightVAO"]);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 36, instances.objects.size());
	glBindVertexArray(0);
}

void LightCubes::add(glm::vec3 position, glm::vec3 color)
{
	instances.objects.emplace_back(Transformations{ position });
	colors.emplace_back(color);
}

void LightCubes::erase(unsigned int idx)
{
	instances.objects.erase(instances.objects.begin() + idx);
	colors.erase(colors.begin() + idx);
}

void LightCubes::resize(size_t size)
{
	instances.objects.resize(size);
	colors.resize(size);
}

size_t LightCubes::getSize()
{
	return size_t(instances.objects.size());
}

std::pair<Transformations&, glm::vec3&> LightCubes::operator[](unsigned int idx)
{
	return std::pair<Transformations&, glm::vec3&>(
		instances.objects[idx],
		colors[idx]
	);
}
