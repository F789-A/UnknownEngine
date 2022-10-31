#pragma once

#include "Texture.h"

#include <glad/glad.h>  
#include <GLFW\glfw3.h>

using uint = unsigned int;

class I_GLTexture
{
protected:
	I_GLTexture() = default;
public: 
	virtual int Type() const = 0;
	virtual int Id() const = 0;

	virtual ~I_GLTexture() = default;

	I_GLTexture operator=(const I_GLTexture&) = delete;
	I_GLTexture(const I_GLTexture&) = delete;
	I_GLTexture(I_GLTexture&& other) = delete;
	I_GLTexture& operator=(I_GLTexture&& other) = delete;
};

class GLTexture: public I_GLTexture
{
private:
	bool HaveGPUResources = true;
	uint TexId;
public:

	GLTexture(int x, int y);

	int Type() const override
	{
		return GL_TEXTURE_2D;
	}

	int Id() const override
	{
		return TexId;
	}

	GLTexture(const Texture& texture, int wrapS = GL_REPEAT, int wrapT = GL_REPEAT, bool generateMipmap = true,
		int minFilter = GL_LINEAR_MIPMAP_LINEAR, int magFilter = GL_LINEAR);
	
	GLTexture operator=(const GLTexture&) = delete;
	GLTexture(const GLTexture&) = delete;

	GLTexture(GLTexture&& other) noexcept;
	GLTexture& operator=(GLTexture&& other) noexcept;

	~GLTexture() override;
};

class GLCubemapTexture: public I_GLTexture
{
private:
	bool HaveGPUResources = true;
	uint TexId;
public:
	int Type() const override
	{
		return GL_TEXTURE_CUBE_MAP;
	}

	int Id() const override
	{
		return TexId;
	}

	GLCubemapTexture(const std::vector<Texture*>& textures, int minFilter = GL_LINEAR, int magFilter = GL_LINEAR,
		int wrapS = GL_CLAMP_TO_EDGE, int wrapT = GL_CLAMP_TO_EDGE, int wrapR = GL_CLAMP_TO_EDGE);

	GLCubemapTexture operator=(const GLTexture&) = delete;
	GLCubemapTexture(const GLTexture&) = delete;

	GLCubemapTexture(GLCubemapTexture&& other) noexcept;
	GLCubemapTexture& operator=(GLCubemapTexture&& other) noexcept;

	~GLCubemapTexture() override;
};