#pragma once

#include <iostream>

#include <glm/glm.hpp>

#include <string>
#include <vector>

using uint = unsigned int;

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangentVec;
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

    Mesh(std::string name, std::vector<Vertex> vertices, std::vector<uint> indices) :
        Name(name),
        Vertices(vertices),
        Indices(indices)
    {}
};

//class PermanentMesh: public Mesh{}