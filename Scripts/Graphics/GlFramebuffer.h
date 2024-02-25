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

	GlFramebuffer(int width, int heignt);

	~GlFramebuffer();
};