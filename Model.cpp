#include "Model.h"

Model::Model(Model&& other) noexcept:
	Meshes(),
	Directory()
{
	Meshes = std::move(other.Meshes);
	Directory = std::move(other.Directory);
}

Model& Model::operator=(Model&& other) noexcept
{
    if (this != &other)
    {
		Meshes = std::move(other.Meshes);
		Directory = std::move(other.Directory);
    }
    return *this;
}

