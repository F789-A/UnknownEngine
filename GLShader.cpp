#include "GLShader.h"


GLShader::GLShader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
    Program = glCreateProgram();
    CreateAndAttachShader(vertexPath, GL_VERTEX_SHADER);
    CreateAndAttachShader(fragmentPath, GL_FRAGMENT_SHADER);
    glLinkProgram(Program);

    Validate();
}

GLShader::GLShader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath)
{
    Program = glCreateProgram();
    CreateAndAttachShader(vertexPath, GL_VERTEX_SHADER);
    CreateAndAttachShader(geometryPath, GL_GEOMETRY_SHADER);
    CreateAndAttachShader(fragmentPath, GL_FRAGMENT_SHADER);
    glLinkProgram(Program);

    Validate();
}

void GLShader::Validate()
{
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(Program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(Program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        GlAssert(false, "");
    }
}

void GLShader::CreateAndAttachShader(const GLchar* pathShader, const GLint typeShader)
{
    std::string shaderCode;
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::badbit);
    try
    {
        shaderFile.open(pathShader);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        shaderCode = shaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        GlAssert(false, "");
    }
    const GLchar* cShaderCode = shaderCode.c_str();

    GLint success;
    GLchar infoLog[512];

    GLuint shader;
    shader = glCreateShader(typeShader);
    glShaderSource(shader, 1, &cShaderCode, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::string type = "VERTEX";
        if (typeShader == GL_VERTEX_SHADER)
        {
            type = "VERTEX";
        } 
        else  if (typeShader == GL_GEOMETRY_SHADER)
        {
            type = "GEOMETRY";
        }
        else  if (typeShader == GL_FRAGMENT_SHADER)
        {
            type = "FRAGMENT";
        }
        std::cout << "ERROR::SHADER::" << type <<"::COMPILATION_FAILED\n" << infoLog << std::endl;
        GlAssert(false, "");
    };

    glAttachShader(Program, shader);

    glDeleteShader(shader);
}

void GLShader::Use()
{
    if (DepthTest)
    {
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
    glDepthFunc(GL_LEQUAL);

    if (CullFace)
    {
        glEnable(GL_CULL_FACE);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }
    glCullFace(CullMode);
    glFrontFace(FrontFaceMode);

    if (Blend)
    {
        glEnable(GL_BLEND);
    }
    else
    {
        glDisable(GL_BLEND);
    }
    glBlendFunc(BlendSourceFunc, BlendDestinationFunc);

    glUseProgram(this->Program);
}

void GLShader::SetVec3(std::string name, glm::vec3 vec)
{
    this->SetVec3(name, vec[0], vec[1], vec[2]);
}

void GLShader::SetVec3(std::string name, GLfloat a, GLfloat b, GLfloat c)
{
    GLint objectParam = glGetUniformLocation(this->Program, name.c_str());
    glUniform3f(objectParam, a, b, c);
}

void GLShader::SetFloat(std::string name, GLfloat a)
{
    GLint objectParam = glGetUniformLocation(this->Program, name.c_str());
    glUniform1f(objectParam, a);
}