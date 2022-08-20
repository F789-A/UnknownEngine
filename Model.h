#pragma once

#include <Mesh.h>

#include <filesystem>

class Model
{
public:
    std::string Directory;

    std::vector<Mesh> Meshes;
    //std::vector<MaterialInfo> MatInfo;

public:
    Model() = default;

    Model(const Model&) = delete;
    Model operator=(const Model&) = delete;

    Model(Model&& other) noexcept;
    Model& operator=(Model&& other) noexcept;


    ~Model() = default;
};