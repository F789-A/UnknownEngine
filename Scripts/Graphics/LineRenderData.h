#pragma once
#include <vector>
#include <glm/vec2.hpp>

class LineRenderData
{
public:
	std::vector<glm::vec2> Vertices;
	std::vector<int> Indeces;
};