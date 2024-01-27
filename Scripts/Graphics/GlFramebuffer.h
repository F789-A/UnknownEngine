#pragma once
#include <glad/glad.h> 
#include <GLFW\glfw3.h>
#include "GLTexture.h"

class GlFramebuffer
{
public:
	GLuint fbo;
	GLTexture texture;
	GLuint rbo;

	GlFramebuffer() : texture(800, 600, 4)
	{
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.Id(), 0);

		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	~GlFramebuffer()
	{
		glDeleteFramebuffers(1, &fbo);
	}
};