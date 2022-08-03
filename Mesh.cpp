#include "Mesh.h"

Mesh::Mesh(const Mesh& other): 
	Name(other.Name),
	Vertices(other.Vertices),
	Indices(other.Indices)
{
	std::cout << "Mesh " << Name << " was copy by contructor" << std::endl;
}

Mesh Mesh::operator=(const Mesh& other)
{
	if (this != &other)
	{
		Name = other.Name;
		Vertices = other.Vertices;
		Indices = other.Indices;
	}
	std::cout << "Mesh " << Name << " was copy by operator \'=\'" << std::endl;
	return *this;
}

Mesh::Mesh(Mesh&& other) noexcept :
	Name(std::move(other.Name)),
	Vertices(std::move(other.Vertices)),
	Indices(std::move(other.Indices))
{}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
	if (this != &other)
	{
		Name = std::move(other.Name);
		Vertices = std::move(other.Vertices);
		Indices = std::move(other.Indices);
	}
	return *this;
}
