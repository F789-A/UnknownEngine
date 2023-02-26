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
    enum RawFlags
    {
        Dynamic = 1
    };

    GLMesh() = default;
    GLMesh(const Mesh& mesh);
    GLMesh(const std::vector<float>& rawData, const std::vector<GLuint>& ind);

    GLMesh(const GLMesh&) = delete;
    GLMesh operator=(const GLMesh&) = delete;
    GLMesh(GLMesh&& other) noexcept;
    GLMesh& operator=(GLMesh&& other) noexcept;
    ~GLMesh();

    void SetData(const std::vector<float>& rawData, int offset = 0);

    void Draw(const GLMaterial& material, const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale);
    void Draw(const GLMaterial& material, const glm::mat4& transformMatrix);
    //TODO: void DrawInstance(Shader& shader);
};

/* TODO: class GLMeshEditable: public GLMesh
* {
* Mesh* LinkedMesh;
* }
*/