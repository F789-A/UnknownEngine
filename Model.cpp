#include "Model.h"

Model::Model(Model&& other) noexcept:
	Meshes(),
	//Textures(),
	Directory()
{
	Meshes = std::move(other.Meshes);
	//Textures = std::move(other.Textures);
	Directory = std::move(other.Directory);
}

Model& Model::operator=(Model&& other) noexcept
{
    if (this != &other)
    {
		Meshes = std::move(other.Meshes);
		//Textures = std::move(other.Textures);
		Directory = std::move(other.Directory);
    }
    return *this;
}

