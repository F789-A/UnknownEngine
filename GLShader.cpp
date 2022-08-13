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
        //std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
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
        Singleton<Logger> logger;
        logger->Log("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n", pathShader, "\n");
        //std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
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
        std::string type = "---";
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
        Singleton<Logger> logger;
        logger->Log("ERROR::SHADER::", type, "::COMPILATION_FAILED\n", infoLog, "\n");
        //std::cout << "ERROR::SHADER::" << type <<"::COMPILATION_FAILED\n" << infoLog << std::endl;
        GlAssert(false, "");
    };

    glAttachShader(Program, shader);

    glDeleteShader(shader);
}

GLShader& GLShader::operator=(GLShader&& other) noexcept
{
    Program = other.Program;

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

GLShader::GLShader(GLShader&& other) noexcept:Program ( other.Program), FrontFaceMode(other.FrontFaceMode),
    DepthTest (other.DepthTest), CullFace (other.CullFace), Blend(other.Blend), CullMode(other.CullMode),
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
    DepthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
    glDepthFunc(DepthFunc);

    CullFace ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
    glCullFace(CullMode);
    glFrontFace(FrontFaceMode);

    Blend ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
    glBlendFunc(BlendSourceFunc, BlendDestinationFunc);

    glUseProgram(this->Program);
}

void GLShader::SetVec3(const std::string& name, glm::vec3 vec)
{
    GLint objectParam = glGetUniformLocation(Program, name.c_str());
    glUniform3f(objectParam, vec[0], vec[1], vec[2]);
}

void GLShader::SetFloat(const std::string& name, GLfloat a)
{
    GLint objectParam = glGetUniformLocation(Program, name.c_str());
    glUniform1f(objectParam, a);
}

void GLShader::SetInt(const std::string& name, GLint a)
{
    GLint objectParam = glGetUniformLocation(Program, name.c_str());
    glUniform1i(objectParam, a);
}

void GLShader::SetTexture(const std::string& name, const GLTexture& a, int target)
{
    glActiveTexture(GL_TEXTURE0 + target);
    glUniform1i(glGetUniformLocation(Program, name.c_str()), target);
    glBindTexture(a.Type(), a.Id); // костыль

    glActiveTexture(GL_TEXTURE0);
}