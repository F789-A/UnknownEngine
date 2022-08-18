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
}

GLMesh::GLMesh(GLMesh&& other) noexcept : VAO(other.VAO), VBO(other.VBO), EBO(other.EBO), IndicesSize(other.IndicesSize), HaveGPUResources(true)
{
    other.HaveGPUResources = false;
}
GLMesh& GLMesh::operator=(GLMesh&& other) noexcept
{
    VAO = other.VAO;
    VBO = other.VBO;
    EBO = other.EBO;
    IndicesSize = other.IndicesSize;
    HaveGPUResources = true;
    other.HaveGPUResources = false;
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
    model = glm::scale(model, scale);
    model = glm::mat4_cast(rotation) * model;
    model = glm::translate(model, position);

    glUniformMatrix4fv(glGetUniformLocation(material.Shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, IndicesSize, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void GLMesh::Draw(GLShader& shader, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
    glUseProgram(shader.Program);

    /*for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);

        glUniform1i(glGetUniformLocation(shader.Program, ("material.texture_" + textures[i].name).c_str()), i);

        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);*/
    glDisable(GL_CULL_FACE);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    model = glm::rotate(model, rotation.z, glm::vec3(0, 0, 1));
    model = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));
    model = glm::rotate(model, rotation.x, glm::vec3(1, 0, 0));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, IndicesSize, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}