#pragma once

#include <glad/glad.h> 
#include <GLFW\glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"

#include "GLShader.h"

#include "GLMaterial.h"

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

    ~GLMesh();


    void Draw(GLShader& shader, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

    void Draw(const GLMaterial& material, const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale);

    //TODO: void DrawInstance(Shader& shader);
};

/* TODO: class GLMeshEditable: public GLMesh
* {
* Mesh* LinkedMesh;
* }
*/