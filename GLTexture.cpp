#include "GLTexture.h"

#include <iostream>

GLTexture::GLTexture(const Texture& texture)
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
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

GLCubemapTexture::GLCubemapTexture(const std::vector<Texture*>& textures)
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
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}