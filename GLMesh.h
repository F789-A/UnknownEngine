#pragma once

#include "Mesh.h"
#include "GLMaterial.h"

#include <glad/glad.h> 
#include <GLFW\glfw3.h>

#include <glm/glm.hpp>

using uint = unsigned int;

class GLMesh
{
public:
    enum class DrawFlags : GLenum
    {
        StaticDraw = GL_STATIC_DRAW,
        DynamicDraw = GL_DYNAMIC_DRAW,
        StreamDraw = GL_STREAM_DRAW
    };

    enum class GeometryTypes : GLenum
    {
        Triangles = GL_TRIANGLES,
        Lines = GL_LINES,
        Points = GL_POINTS
    };

public:
    GLMesh(const std::vector<Vertex3D>& vertexData, const std::vector<GLuint>& indices, 
        GeometryTypes geometryType = GeometryTypes::Triangles, DrawFlags drawFlags = DrawFlags::StaticDraw);
    GLMesh(const std::vector<Vertex2D>& vertexData, const std::vector<GLuint>& indices,
        GeometryTypes geometryType = GeometryTypes::Triangles, DrawFlags drawFlags = DrawFlags::StaticDraw);
    GLMesh(const std::vector<Vertex>& vertexData, const std::vector<GLuint>& indices,
        GeometryTypes geometryType = GeometryTypes::Triangles, DrawFlags drawFlags = DrawFlags::StaticDraw);

    GLMesh();
    GLMesh(const GLMesh&) = delete;
    GLMesh operator=(const GLMesh&) = delete;
    GLMesh(GLMesh&& other) noexcept;
    GLMesh& operator=(GLMesh&& other) noexcept;
    ~GLMesh();

    void SetData(const std::vector<float>& rawData, int offset = 0);
    void SetIndices(const std::vector<GLuint>& indices);

    void Draw(const GLMaterial& material, const glm::mat4& transformMatrix);
    //TODO: void DrawInstance(Shader& shader);

private:
    uint VAO;
    uint VBO;
    uint EBO;
    int IndicesSize;
    bool HaveGPUResources = false;

    GeometryTypes GeometryType;
};

/* TODO: class GLMeshEditable: public GLMesh
* {
* Mesh* LinkedMesh;
* }
*/