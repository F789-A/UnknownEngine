#pragma once

#include <glad/glad.h> 
#include <GLFW\glfw3.h>

#include <vector>
#include <string>

#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 position;
};

struct Vertex2D
{
    glm::vec3 position;
    glm::vec2 texCoords;
};

struct Vertex3D
{
    glm::vec3 position;
    glm::vec2 texCoords;
    glm::vec3 normal;
    glm::vec3 tangentVec;
};

class Mesh 
{
public:
    std::vector<Vertex3D> Vertices;
    std::vector<GLuint> Indices;
    std::string Name;
};