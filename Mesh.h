#pragma once

#include <iostream>

#include <glm/glm.hpp>

#include <string>
#include <vector>
#include "Logger.h"

using uint = unsigned int;

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
    std::vector<unsigned int> Indices;
    std::string Name;

    Mesh(){}

    Mesh(const Mesh& other);
    Mesh operator=(const Mesh& other);

    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    Mesh(const std::string& name, const std::vector<Vertex3D>& vertices, const std::vector<unsigned int>& indices);
};