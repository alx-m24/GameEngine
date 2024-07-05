#include "LightingSystem.hpp"

void LightingSystem::update(Shader& shader)
{
	for (int i = 0; i < dirLights.size(); ++i) {
		std::string name = "dirLight[" + std::to_string(i) + "].";
		DirectionalLight& dirLight = dirLights[i];

		shader.setVec3(name + "direction",dirLight.direction);
		shader.setVec3(name + "ambient", dirLight.ambient);
		shader.setVec3(name + "diffuse", dirLight.diffuse);
		shader.setVec3(name + "specular", dirLight.specular);
		shader.setVec3(name + "color", dirLight.color);
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
	}

	for (int i = 0; i < SpotLights.size(); ++i) {
		std::string name = "spotLights[" + std::to_string(i) + "].";
		SpotLight& spotLight = SpotLights[i];

		shader.setVec3(name + "position", spotLight.position);
		shader.setVec3(name + "direcion", spotLight.direcion);
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
}
