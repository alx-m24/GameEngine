#pragma once
#include <unordered_map>
#include <string>
#include <filesystem>

#include "Textures/Textures.hpp"
#include "Shaders/Shader.hpp"
#include "Model.hpp"

namespace Resources {
	extern std::unordered_map<std::string, unsigned int> textures;
	extern std::unordered_map<std::string, Model> models;
	extern std::unordered_map<std::string, Shader> shaders;
	extern std::unordered_map<std::string, unsigned int> buffers;
	extern unsigned int ShadowMapResolution;
	extern const std::string PATH;
}

namespace Vertices {
	extern float cube[];
}