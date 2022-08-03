#pragma once

#include <glad/glad.h>  
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "EngineCore/GlAssert.h"

class GLShader
{
private:
    void CreateAndAttachShader(const GLchar* path, const GLint typeShader);
    void Validate();

public:
    GLuint Program;

    bool DepthTest = true;
    bool CullFace = true;
    bool Blend = false;
    int CullMode = GL_BACK;
    const int FrontFaceMode = GL_CCW;
    int BlendSourceFunc = GL_SRC_ALPHA;
    int BlendDestinationFunc = GL_ONE_MINUS_SRC_ALPHA;

    GLShader(const GLchar* vertexPath, const GLchar* fragmentPath);
    GLShader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath);

    GLShader operator=(const GLShader&) = delete;
    GLShader(const GLShader&) = delete;

    ~GLShader() = default;


    void Use();
    void SetVec3(std::string name, glm::vec3 vec);
    void SetVec3(std::string name, GLfloat a, GLfloat b, GLfloat c);
    void SetFloat(std::string name, GLfloat a);
};