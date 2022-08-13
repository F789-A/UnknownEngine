#pragma once

#include "Texture.h"

#include <glad/glad.h>  
#include <GLFW\glfw3.h>

using uint = unsigned int;

class GLTexture
{
protected:
	GLTexture() = default;
	bool HaveGPUResources = true;

public:
	uint Id;

	GLTexture(const Texture& texture, int wrapS = GL_REPEAT, int wrapT = GL_REPEAT, bool generateMipmap = true,
		int minFilter = GL_LINEAR_MIPMAP_LINEAR, int magFilter = GL_LINEAR);
	
	GLTexture operator=(const GLTexture&) = delete;
	GLTexture(const GLTexture&) = delete;

	GLTexture(GLTexture&& other) noexcept;
	GLTexture& operator=(GLTexture&& other) noexcept;

	virtual ~GLTexture();
};

class GLCubemapTexture: public GLTexture
{
public:

	GLCubemapTexture(const std::vector<Texture*>& textures, int minFilter = GL_LINEAR, int magFilter = GL_LINEAR,
		int wrapS = GL_CLAMP_TO_EDGE, int wrapT = GL_CLAMP_TO_EDGE, int wrapR = GL_CLAMP_TO_EDGE);

	GLCubemapTexture operator=(const GLTexture&) = delete;
	GLCubemapTexture(const GLTexture&) = delete;

	GLCubemapTexture(GLCubemapTexture&& other) noexcept;
	GLCubemapTexture& operator=(GLCubemapTexture&& other) noexcept;

	~GLCubemapTexture() {};
};