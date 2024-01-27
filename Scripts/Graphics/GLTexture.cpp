#include "GLTexture.h"

GLTexture::GLTexture(int x, int y, int numberComponents):
	HaveGPUResources(true)
{
	glGenTextures(1, &TexId);
	glBindTexture(GL_TEXTURE_2D, TexId);

	GLenum format = GL_RED;
	if (numberComponents == 1)
	{
		format = GL_RED;
	}
	else if (numberComponents == 3)
	{
		format = GL_RGB;
	}
	else if (numberComponents == 4)
	{
		format = GL_RGBA;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, format, x, y, 0, format, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLTexture::GLTexture(const Texture& texture, int wrapS, int wrapT, bool generateMipmap, 
	int minFilter, int magFilter):
	HaveGPUResources(true)
{
	glGenTextures(1, &TexId);

	GLenum format = GL_RED;
	if (texture.NumberComponents == 1)
	{
		format = GL_RED;
	}
	else if (texture.NumberComponents == 3)
	{
		format = GL_RGB;
	}
	else if (texture.NumberComponents == 4)
	{
		format = GL_RGBA;
	}

	glBindTexture(GL_TEXTURE_2D, TexId);
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, format, texture.Width, texture.Height, 0, format, GL_UNSIGNED_BYTE, &texture.Data[0]);

	if (generateMipmap)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
}

GLTexture::GLTexture(GLTexture&& other) noexcept :
	TexId(other.TexId),
	HaveGPUResources(true)
{
	other.HaveGPUResources = false;
}

GLTexture& GLTexture::operator=(GLTexture&& other) noexcept
{
	TexId = other.TexId;
	HaveGPUResources = true;
	other.HaveGPUResources = false;
	return *this;
}

GLTexture::~GLTexture()
{
	if (HaveGPUResources)
	{
		glDeleteTextures(1, &TexId);
	}
}

GLCubemapTexture::GLCubemapTexture(const std::vector<Texture*>& textures, 
	int minFilter, int magFilter, int wrapS, int wrapT, int wrapR)
{
	glGenTextures(1, &TexId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TexId);

	for (uint i = 0; i < textures.size(); i++)
	{
		GLenum format = GL_RED;
		if (textures[i]->NumberComponents == 1)
		{
			format = GL_RED;
		}
		else if (textures[i]->NumberComponents == 3)
		{
			format = GL_RGB;
		}
		else if (textures[i]->NumberComponents == 4)
		{
			format = GL_RGBA;
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, textures[i]->Width, textures[i]->Height, 0, format, GL_UNSIGNED_BYTE, &(textures[i]->Data[0]));
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrapT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrapR);
}

GLCubemapTexture::GLCubemapTexture(GLCubemapTexture&& other) noexcept :
	TexId(other.TexId), 
	HaveGPUResources(true)
{
	other.HaveGPUResources = false;
}
GLCubemapTexture& GLCubemapTexture::operator=(GLCubemapTexture&& other) noexcept
{
	TexId = other.TexId;
	HaveGPUResources = true;
	other.HaveGPUResources = false;
	return *this;
}

GLCubemapTexture::~GLCubemapTexture()
{
	if (HaveGPUResources)
	{
		glDeleteTextures(1, &TexId);
	}
}