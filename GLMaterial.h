#pragma once
#include "GLShader.h"

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>  
#include <GLFW\glfw3.h>

#include <filesystem>

class GLMaterial
{
public:
	GLShader* Shader;
	GLMaterial() = default;

	GLMaterial(GLShader& shader);

	void Use();
	void SetVec3(std::string name, glm::vec3 vec);
	void SetVec3(std::string name, GLfloat a, GLfloat b, GLfloat c);
	void SetFloat(std::string name, GLfloat a);
};