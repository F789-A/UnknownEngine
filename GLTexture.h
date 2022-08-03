#pragma once

#include "Texture.h"
#include <glad/glad.h>  
#include <GLFW\glfw3.h>

using uint = unsigned int;

class GLTexture
{
protected:
	GLTexture() = default;

public:
	uint Id;

	GLTexture(const Texture& texture);
};

class GLCubemapTexture: public GLTexture
{
public:
	GLCubemapTexture() = default;

	GLCubemapTexture(const std::vector<Texture*>& textures);
};