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

	Texture(int w, int h, int num = 3): Width(w), Height(h), NumberComponents(num)
	{
		Data = std::vector<ubyte>(w * h * num);
	}

	void set(int x, int y, ubyte val)
	{
		Data[y * Width + x] = val;
	}

	~Texture() = default;
};