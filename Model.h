#pragma once

#include <Mesh.h>

#include <filesystem>

/*struct Texture
{
    unsigned int id;
    std::string name;
    std::string path;
};*/

class Model
{
public:
    std::string Directory;

    std::vector<Mesh> Meshes;
    //std::vector<Texture> Textures;

    //std::vector<Shader> Shaders
public:
    Model() = default;

    Model(const Model&) = delete;
    Model operator=(const Model&) = delete;

    Model(Model&& other) noexcept;
    Model& operator=(Model&& other) noexcept;


    ~Model() = default;
};