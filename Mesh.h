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
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangentVec;
};

struct Vertex2D
{
    glm::vec3 position;
    glm::vec2 texCoords;
};

class Mesh 
{
public:
    std::string Name;
    std::vector<Vertex> Vertices;
    std::vector<uint> Indices;

    Mesh(){}

    Mesh(const Mesh& other);
    Mesh operator=(const Mesh& other);

    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    Mesh(std::string name, std::vector<Vertex> vertices, std::vector<uint> indices);
};

class Mesh2D
{
public:
    std::vector<Vertex2D> Vertices;
    std::vector<uint> Indices;

    Mesh2D() {}

    Mesh2D(const Mesh2D& other);
    Mesh2D operator=(const Mesh2D& other);

    Mesh2D(Mesh2D&& other) noexcept;
    Mesh2D& operator=(Mesh2D&& other) noexcept;

    Mesh2D(std::vector<Vertex> vertices, std::vector<uint> indices);
};

//TODO class PermanentMesh: public Mesh{}