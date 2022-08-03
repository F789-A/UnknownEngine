#pragma once
#include <vector>
#include <string>

using ubyte = unsigned char;
using uint = unsigned int;

class Texture
{
public:
	std::vector<ubyte> Data;
	int Width = 0;
	int Height = 0;
	int NumberComponents = 0;

	Texture() = delete;

	Texture(std::string path);

	~Texture() = default;
};