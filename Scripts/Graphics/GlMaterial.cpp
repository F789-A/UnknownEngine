#include "GLMaterial.h"

GLMaterial::GLMaterial(GLShader& shader): Shader(&shader)
{}

void GLMaterial::Use() const 
{
	Shader->Use();
	for (auto l : ParametersInt)
	{
		Shader->SetInt(l.first, l.second);
	}
	for (auto l : ParametersFloat)
	{
		Shader->SetFloat(l.first, l.second);
	}
	for (auto l : ParametersVec3)
	{
		Shader->SetVec3(l.first, l.second);
	}
	int i = 0;
	for (auto l : Textures)
	{
		Shader->SetTexture(l.first, *(l.second), i);
		i++;
	}
	i = 0;
	for (auto l : CubemapTextures)
	{
		Shader->SetTexture(l.first, *(l.second), i);
		i++;
	}

}