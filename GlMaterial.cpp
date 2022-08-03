#include "GLMaterial.h"

GLMaterial::GLMaterial(GLShader& shader): Shader(&shader)
{}

void GLMaterial::Use()
{
	Shader->Use();
}