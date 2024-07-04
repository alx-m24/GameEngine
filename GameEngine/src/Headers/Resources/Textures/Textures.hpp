//#pragma once
#ifndef TEXURE_H
#define TEXURE_H

#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>
#include <unordered_map>

struct Texture
{
	unsigned int id;
	std::string type;
	std::string path;
};

unsigned int loadTexture(char const* path);
unsigned int TextureFromFile(const char* path, const std::string& directory);

extern std::unordered_map<std::string, unsigned int> textures;
#endif // TEXTURE_H