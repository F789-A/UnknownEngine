#pragma once

#include <glad/glad.h>  
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>

#include <string>
//#include <iostream>
#include <fstream>
#include <sstream>

#include "GLTexture.h"
#include "Logger.h"

class GLShader
{
private:
    void CreateAndAttachShader(const GLchar* path, const GLint typeShader);
    void Validate();
    bool HaveResources = true;
public:
    GLuint Program;

    int FrontFaceMode = GL_CCW;
    bool DepthTest = true;
    bool CullFace = true;
    bool Blend = false;
    int CullMode = GL_BACK;
    int BlendSourceFunc = GL_SRC_ALPHA;
    int BlendDestinationFunc = GL_ONE_MINUS_SRC_ALPHA;
    int DepthFunc = GL_LEQUAL;

    GLShader(const GLchar* vertexPath, const GLchar* fragmentPath);
    GLShader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath);

    GLShader& operator=(const GLShader&) = delete;
    GLShader(const GLShader&) = delete;

    GLShader& operator=(GLShader&&) noexcept;
    GLShader(GLShader&&) noexcept;

    ~GLShader();

    void Use();
    void SetVec3(const std::string& name, glm::vec3 vec);
    void SetFloat(const std::string& name, GLfloat a);
    void SetInt(const std::string& name, GLint a);
    void SetTexture(const std::string& name, const GLTexture& a, int target);
};