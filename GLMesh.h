#pragma once

#include <glad/glad.h> 
#include <GLFW\glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"

#include "GLShader.h"

using uint = unsigned int;

class GLMesh
{
public:
    uint VAO;
    uint VBO;
    uint EBO;
    int IndicesSize;
    bool HaveGPUResources = false;

public:

    GLMesh() = default;
    GLMesh(const Mesh& mesh);

    GLMesh(const GLMesh&) = delete;
    GLMesh operator=(const GLMesh&) = delete;

    GLMesh(GLMesh&& other) noexcept;
    GLMesh& operator=(GLMesh&& other) noexcept;

    //TODO деструктор освобождающий ресурсы видеокарты
    ~GLMesh();

    //void Draw(GLShader& shader, glm::mat3 modelMat, std::vector<Texture> textures);

    void Draw(GLShader& shader, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
    //void DrawInstance(Shader& shader);
};

/*class GLMeshEditable: public GLMesh
* {
* Mesh* LinkedMesh;
* }
*/