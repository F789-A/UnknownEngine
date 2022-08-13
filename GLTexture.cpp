#include "GLTexture.h"

GLTexture::GLTexture(const Texture& texture, int wrapS, int wrapT, bool generateMipmap, int minFilter, int magFilter):
	HaveGPUResources(true)
{
	glGenTextures(1, &Id);

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

	glBindTexture(GL_TEXTURE_2D, Id);
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

GLTexture::GLTexture(GLTexture&& other) noexcept : Id(other.Id), HaveGPUResources(true)
{
	other.HaveGPUResources = false;
}

GLTexture& GLTexture::operator=(GLTexture&& other) noexcept
{
	Id = other.Id;
	HaveGPUResources = true;
	other.HaveGPUResources = false;
	return *this;
}

GLTexture::~GLTexture()
{
	if (HaveGPUResources)
	{
		glDeleteTextures(1, &Id);
	}
}

GLCubemapTexture::GLCubemapTexture(const std::vector<Texture*>& textures,
	int minFilter, int magFilter, int wrapS, int wrapT, int wrapR)
{
	glGenTextures(1, &Id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Id);

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

GLCubemapTexture::GLCubemapTexture(GLCubemapTexture&& other) noexcept :GLTexture(std::move(other)) {}
GLCubemapTexture& GLCubemapTexture::operator=(GLCubemapTexture&& other) noexcept
{
	Id = other.Id;
	HaveGPUResources = true;
	other.HaveGPUResources = false;
	return *this;
}