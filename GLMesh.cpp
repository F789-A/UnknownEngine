#include "GLMesh.h"
#include "GraphicCore.h"

GLMesh::GLMesh(const Mesh& mesh)
{
    HaveGPUResources = true;
    IndicesSize = mesh.Indices.size();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, mesh.Vertices.size() * sizeof(Vertex), &(mesh.Vertices[0]), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.Indices.size() * sizeof(unsigned int), &(mesh.Indices[0]), GL_STATIC_DRAW);
   
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangentVec));
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLMesh::GLMesh(const std::vector<float>& rawData, const std::vector<GLuint>& ind)
{
    HaveGPUResources = true;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    IndicesSize = ind.size();

    glBufferData(GL_ARRAY_BUFFER, rawData.size() * sizeof(float), &(rawData[0]), GL_STREAM_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndicesSize * sizeof(unsigned int), &(ind[0]), GL_STREAM_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*  sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLMesh::SetData(const std::vector<float>& rawData, int offset)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, rawData.size() * sizeof(float), &(rawData[0]));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLMesh::GLMesh(GLMesh&& other) noexcept : VAO(other.VAO), VBO(other.VBO), EBO(other.EBO), IndicesSize(other.IndicesSize), HaveGPUResources(true)
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
    }
    return *this;
}

GLMesh::~GLMesh()
{
    if (HaveGPUResources)
    {
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteBuffers(1, &VAO);
    }
}

void GLMesh::Draw(const GLMaterial& material, const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale)
{
    material.Use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(glm::mat4(1.0f), scale);
    //model = glm::mat4_cast(rotation) * model;
    model = glm::translate(glm::mat4(1.0f), position) * model;

    glUniformMatrix4fv(glGetUniformLocation(material.Shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, IndicesSize, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void GLMesh::Draw(const GLMaterial& material, const glm::mat4& transformMatrix)
{
    material.Use();

    glUniformMatrix4fv(glGetUniformLocation(material.Shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(transformMatrix));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, IndicesSize, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}