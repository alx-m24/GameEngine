#include <unordered_map>
#include <string>

#include "Textures/Textures.hpp"
#include "Shaders/Shader.hpp"
#include "Model.hpp"

namespace Resources {
	std::unordered_map<std::string, unsigned int> textures;
	std::unordered_map<std::string, Model> models;
	std::unordered_map<std::string, Shader> shaders;
}