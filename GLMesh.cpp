#include "GLMesh.h"
#include "GraphicCore.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <numeric>

GLMesh::GLMesh() : HaveGPUResources(false) 
{}

GLMesh::GLMesh(const std::vector<Vertex3D>& vertexData, const std::vector<GLuint>& indices,
    GeometryTypes geometryType, DrawFlags drawFlags) : GeometryType(geometryType)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    HaveGPUResources = true;

    IndicesSize = indices.size();
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vertex3D), &(vertexData[0]), static_cast<GLenum>(drawFlags));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &(indices[0]), static_cast<GLenum>(drawFlags));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, texCoords));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, normal));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, tangentVec));
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLMesh::GLMesh(const std::vector<Vertex2D>& vertexData, const std::vector<GLuint>& indices,
    GeometryTypes geometryType, DrawFlags drawFlags) : GeometryType(geometryType)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    HaveGPUResources = true;

    IndicesSize = indices.size();
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vertex3D), &(vertexData[0]), static_cast<GLenum>(drawFlags));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &(indices[0]), static_cast<GLenum>(drawFlags));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, texCoords));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLMesh::GLMesh(const std::vector<Vertex>& vertexData, const std::vector<GLuint>& indices,
    GeometryTypes geometryType, DrawFlags drawFlags) : GeometryType(geometryType)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    HaveGPUResources = true;

    IndicesSize = indices.size();
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vertex3D), &(vertexData[0]), static_cast<GLenum>(drawFlags));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &(indices[0]), static_cast<GLenum>(drawFlags));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLMesh::GLMesh(GLMesh&& other) noexcept : VAO(other.VAO), VBO(other.VBO), EBO(other.EBO), IndicesSize(other.IndicesSize), 
    HaveGPUResources(true), GeometryType(other.GeometryType)
{
    other.HaveGPUResources = false;
}

GLMesh& GLMesh::operator=(GLMesh&& other) noexcept
{
    if (this != &other)
    {
        VAO = other.VAO;
        VBO = other.VBO;
        EBO = other.EBO;
        IndicesSize = other.IndicesSize;
        HaveGPUResources = true;
        other.HaveGPUResources = false;
        GeometryType = other.GeometryType;
    }
    return *this;
}

GLMesh::~GLMesh()
{
    if (HaveGPUResources)
    {
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
    }
}

void GLMesh::SetVertexData(const std::vector<GLfloat>& rawData)
{
    if (!HaveGPUResources)
    {
        throw "";
    }
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, rawData.size() * sizeof(GLfloat), &(rawData[0]));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLMesh::SetIndicesData(const std::vector<GLuint>& indices)
{
    if (!HaveGPUResources)
    {
        throw "";
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(GLuint), &(indices[0]));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLMesh::Draw(const GLMaterial& material, const glm::mat4& transformMatrix)
{
    if (!HaveGPUResources)
    {
        throw "";
    }
    material.Use();

    glUniformMatrix4fv(glGetUniformLocation(material.Shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(transformMatrix));

    glBindVertexArray(VAO);
    glDrawElements(static_cast<GLenum>(GeometryType), IndicesSize, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}