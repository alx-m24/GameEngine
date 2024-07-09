#include "LightingSystem.hpp"

LightingSystem::LightingSystem(glm::vec3 pointPos[], glm::vec3 pointColor[], unsigned int count)
{
	lightCubes = LightCubes(pointPos, pointColor, count);
}

void LightingSystem::update(Shader& shader)
{
	for (int i = 0; i < dirLights.size(); ++i) {
		std::string name = "dirLights[" + std::to_string(i) + "].";
		DirectionalLight& dirLight = dirLights[i];

		shader.setVec3(name + "direction",dirLight.direction);
		shader.setVec3(name + "ambient", dirLight.ambient);
		shader.setVec3(name + "diffuse", dirLight.diffuse);
		shader.setVec3(name + "specular", dirLight.specular);
		shader.setVec3(name + "color", dirLight.color);
	}

	for (int i = 0; i < SpotLights.size(); ++i) {
		std::string name = "spotLights[" + std::to_string(i) + "].";
		SpotLight& spotLight = SpotLights[i];

		shader.setVec3(name + "position", spotLight.position);

		shader.setVec3(name + "direction", spotLight.direcion);

		shader.setVec3(name + "ambient", spotLight.ambient);
		shader.setVec3(name + "diffuse", spotLight.diffuse);
		shader.setVec3(name + "specular", spotLight.specular);

		shader.setFloat(name + "constant", spotLight.constant);
		shader.setFloat(name + "linear", spotLight.linear);
		shader.setFloat(name + "quadratic", spotLight.quadratic);

		shader.setFloat(name + "cutOff", spotLight.cutOff);
		shader.setFloat(name + "outerCutOff", spotLight.outerCutOff);

		shader.setVec3(name + "color", spotLight.color);
	}

	for (int i = 0; i < pointLights.size(); ++i) {
		std::string name = "pointLights[" + std::to_string(i) + "].";
		PointLight& pointLight = pointLights[i];

		shader.setVec3(name + "position", pointLight.position);
		shader.setVec3(name + "ambient", pointLight.ambient);
		shader.setVec3(name + "diffuse", pointLight.diffuse);
		shader.setVec3(name + "specular", pointLight.specular);
		shader.setFloat(name + "constant", pointLight.constant);
		shader.setFloat(name + "linear", pointLight.linear);
		shader.setFloat(name + "quadratic", pointLight.quadratic);
		shader.setVec3(name + "color", pointLight.color);

		Shader& lightCube = shaders["lightCube"];
		lightCube.use();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, pointLight.position);
		model = glm::scale(model, glm::vec3(0.5f));

		lightCube.setMat4("model", model);
		lightCube.setVec3("material.color", pointLight.color);

		glBindVertexArray(buffers["lightVAO"]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		lightCubes.colors[i] = pointLight.color;
		lightCubes.instances.objects[i].position = pointLight.position;

		shader.use();
	}

	lightCubes.update();
}

void LightingSystem::drawLightSources(Shader& shader)
{
	lightCubes.draw(shader);
}

void LightingSystem::addDirectionalLight(DirectionalLight dirLight)
{
	dirLights.emplace_back(dirLight);
}

void LightingSystem::addPointLight(PointLight pointlight)
{
	pointLights.emplace_back(pointlight);
	lightCubes.add(pointlight.position, pointlight.color);
}

void LightingSystem::addSpotlLight(SpotLight spotLight)
{
	SpotLights.emplace_back(spotLight);
}
