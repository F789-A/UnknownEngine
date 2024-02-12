#pragma once
#include <glad/glad.h> 
#include <GLFW\glfw3.h>

#include <glm/glm.hpp>

#include "Assets\Mesh.h"
#include "GLMaterial.h"

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

    void SetVertexData(const std::vector<GLfloat>& rawData);
    void SetIndicesData(const std::vector<GLuint>& indices);

    void Draw(const GLMaterial& material, const glm::mat4& transformMatrix);
    //TODO: void DrawInstance(Shader& shader);

private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    int IndicesSize;
    bool HaveGPUResources = false;

    GeometryTypes GeometryType;
};