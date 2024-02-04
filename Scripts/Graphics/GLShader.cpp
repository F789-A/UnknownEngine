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
        Singleton<Logger> logger;
        logger->Log("ERROR::SHADER::PROGRAM::LINKING_FAILED\n", infoLog, "\n");
        throw std::exception("Error shader program linking failed");
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
        Singleton<Logger> logger;
        logger->Log("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n", pathShader, "\n");
        throw std::exception("Error shader file not read");
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
        std::map<int, std::string> shadType = {
            {GL_VERTEX_SHADER, "VERTEX"},
            {GL_GEOMETRY_SHADER, "GEOMETRY"},
            {GL_FRAGMENT_SHADER, "FRAGMENT"}
        };
        Singleton<Logger> logger;
        logger->Log("ERROR::SHADER::", shadType.at(typeShader), "::COMPILATION_FAILED\n", infoLog, "\n");
        throw std::exception("Shader compilation failed");
    };

    glAttachShader(Program, shader);

    glDeleteShader(shader);
}

GLShader& GLShader::operator=(GLShader&& other) noexcept
{
    Program = other.Program;

    DepthMask = other.DepthMask;
    FrontFaceMode = other.FrontFaceMode;
    DepthTest = other.DepthTest;
    CullFace = other.CullFace;
    Blend = other.Blend;
    CullMode = other.CullMode;
    BlendSourceFunc = other.BlendSourceFunc;
    BlendDestinationFunc = other.BlendDestinationFunc;
    DepthFunc = other.DepthFunc;

    other.HaveResources = false;
    return *this;
}

GLShader::GLShader(GLShader&& other) noexcept:Program ( other.Program), DepthMask(other.DepthMask), FrontFaceMode(other.FrontFaceMode),
    DepthTest(other.DepthTest), CullFace(other.CullFace), Blend(other.Blend), CullMode(other.CullMode),
    BlendSourceFunc(other.BlendSourceFunc), BlendDestinationFunc(other.BlendDestinationFunc), DepthFunc(other.DepthFunc)
{
    other.HaveResources = false;
}

GLShader::~GLShader()
{
    if (HaveResources)
    {
        glDeleteProgram(Program);
    }
}

void GLShader::Use()
{
    if (DepthMask)
    {
        glDepthMask(GL_TRUE);
    }
    else
    {
        glDepthMask(GL_FALSE);
    }
    if (DepthTest)
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(DepthFunc);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
    if (CullFace)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(CullMode);
        glFrontFace(FrontFaceMode);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }
    if (Blend)
    {
        glEnable(GL_BLEND);
        glBlendFunc(BlendSourceFunc, BlendDestinationFunc);
    }
    else
    {
        glDisable(GL_BLEND);
    }

    glUseProgram(this->Program);
}

void GLShader::SetVec3(const std::string& name, glm::vec3 vec)
{
    GLint objectParam = glGetUniformLocation(Program, name.c_str());
    glUseProgram(Program);
    glUniform3f(objectParam, vec[0], vec[1], vec[2]);
}

void GLShader::SetFloat(const std::string& name, GLfloat a)
{
    GLint objectParam = glGetUniformLocation(Program, name.c_str());
    glUseProgram(Program);
    glUniform1f(objectParam, a);
}

void GLShader::SetInt(const std::string& name, GLint a)
{
    GLint objectParam = glGetUniformLocation(Program, name.c_str());
    glUseProgram(Program);
    glUniform1i(objectParam, a);
}

void GLShader::SetTexture(const std::string& name, const I_GLTexture& map, int target)
{
    glUseProgram(Program);
    glActiveTexture(GL_TEXTURE0 + target);
    glUniform1i(glGetUniformLocation(Program, name.c_str()), target);
    glBindTexture(map.Type(), map.Id());

    glActiveTexture(GL_TEXTURE0);
}