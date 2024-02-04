#pragma once
#include <vector>
#include <string>

using ubyte = unsigned char;
using uint = unsigned int;

class Texture
{
public:
	std::vector<ubyte> Data;
	int Width;
	int Height;
	int NumberComponents;

	Texture() = delete;
	Texture(const std::string& path);
	Texture(int w, int h, int num = 3);

	~Texture() = default;
};