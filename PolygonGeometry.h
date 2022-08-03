#pragma once

#include <map>
#include <array>
#include <vector>
#include <list>

#include <glm/glm.hpp>

template <typename T>
int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}


glm::vec3 CalculateTangent(const std::array<glm::vec3, 3>& pos, const std::array<glm::vec2, 3>& uv);

int LeftRotation(glm::vec3 a, glm::vec3 b, glm::vec3 normal);

/*
Получение нормали Newell method
Сложность: O(n)
*/
glm::vec3 PoligonNormal(const std::vector<glm::vec3>& pos, const std::list<unsigned int>& indices);

bool PointInTriangle(const std::vector<glm::vec3>& pos, glm::vec3 point, glm::vec3 normal);

bool PointInTriangle(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec3 point, glm::vec3 normal);

std::vector<unsigned int> Triangulate(const std::vector<glm::vec3>& pos, std::list<unsigned int> indices);