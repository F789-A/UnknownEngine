#pragma once
#include <glad/glad.h>  
#include <GLFW\glfw3.h>

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLShader.h"
#include "GLTexture.h"

#include <filesystem>
#include <map>

class GLMaterial
{
public:
	GLShader* Shader;
	std::map<std::string, GLTexture*> Textures;

	std::map<std::string, int> ParametersInt;
	std::map<std::string, float> ParametersFloat;
	std::map<std::string, glm::vec3> ParametersVec3;

	GLMaterial() {};

	GLMaterial(GLShader& shader);

	~GLMaterial() = default;

	void Use() const;
};