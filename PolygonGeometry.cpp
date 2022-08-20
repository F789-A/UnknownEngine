#include "PolygonGeometry.h"

glm::vec3 CalculateTangent(const std::array<glm::vec3, 3>& pos, const std::array<glm::vec2, 3>& uv)
{
	glm::vec3 edge1 = pos[1] - pos[0];
	glm::vec3 edge2 = pos[2] - pos[0];
	glm::vec2 deltaUV1 = uv[1] - uv[0];
	glm::vec2 deltaUV2 = uv[2] - uv[0];

	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	glm::vec3 tangent;

	tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent = glm::normalize(tangent);

	return tangent;
}

int LeftRotation(glm::vec3 a, glm::vec3 b, glm::vec3 normal)
{
	glm::vec3 axis = glm::cross(a, b);
	float res = glm::dot(axis, normal);
	return sgn<float>(res);
}

/*
Получение нормали Newell method
Сложность: O(n)
*/
glm::vec3 PoligonNormal(const std::vector<glm::vec3>& pos, const std::list<unsigned int>& indices)
{
	glm::vec3 normal(0, 0, 0);
	int indCount = indices.size();
	for (int i = 0; i < indCount; i++)
	{
		glm::vec3 low = pos[*(std::next(indices.begin(), i % indCount))];
		glm::vec3 cen = pos[*(std::next(indices.begin(), (i+1) % indCount))];
		glm::vec3 high = pos[*(std::next(indices.begin(), (i+2) % indCount))];

		normal.x += cen.y * (high.z - low.z);
		normal.y += cen.z * (high.x - low.x);
		normal.z += cen.x * (high.y - low.y);

	}
	return normal;
}

bool PointInTriangle(const std::vector<glm::vec3>& pos, glm::vec3 point, glm::vec3 normal)
{
	int a = LeftRotation(pos[1] - pos[0], point - pos[0], normal);
	int b = LeftRotation(pos[2] - pos[1], point - pos[1], normal);
	int c = LeftRotation(pos[0] - pos[2], point - pos[2], normal);
	return (a > 0 && b > 0 && c > 0) || (a < 0 && b < 0 && c < 0);
}

bool PointInTriangle(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec3 point, glm::vec3 normal)
{
	return PointInTriangle(std::vector<glm::vec3>({ pos1, pos2, pos3 }), point, normal);
}

/*
Неоптимальная реализация триангуляции
Отрезание ушей (Ear Clipping method)
Сложность: O(n^2)
*/
std::vector<unsigned int> Triangulate(const std::vector<glm::vec3>& pos, std::list<unsigned int> indices)
{
	std::list<unsigned int> result;
	glm::vec3 normal = PoligonNormal(pos, indices);
	std::list<unsigned int>::iterator iter[3];
	iter[0] = indices.begin(); // i
	iter[1] = std::next(iter[0]); // i + 1 - center
	iter[2] = std::next(iter[1]); // i + 2

	while (indices.size() != 3)
	{

		bool remove = false;
		if (LeftRotation(pos[*iter[2]] - pos[*iter[1]], pos[*iter[0]] - pos[*iter[1]], normal) > 0) // is convex
		{
			remove = true;
			for (std::list<unsigned int>::iterator iter2 = indices.begin(); iter2 != indices.end(); ++iter2)
			{
				if (iter[0] == iter2 || iter[1] == iter2 || iter[2] == iter2)
				{
					continue;
				}
				if (PointInTriangle(pos[*iter[0]], pos[*iter[1]], pos[*iter[2]], pos[*iter2], normal))
				{
					remove = false;
					break;
				}
			}
		}

		if (remove)
		{
			result.push_back(*iter[0]);
			result.push_back(*iter[1]);
			result.push_back(*iter[2]);

			iter[1] = indices.erase(iter[1]);
		}
		else
		{
			++iter[1];
		}

		if (iter[1] == indices.end())
		{
			iter[1] = indices.begin();
		}

		iter[0] = std::prev(iter[1]);
		iter[2] = std::next(iter[1]);

		if (iter[2] == indices.end())
		{
			iter[2] = indices.begin();
		}
	}

	result.push_back(*indices.begin());
	result.push_back(*std::next(indices.begin(), 1));
	result.push_back(*std::next(indices.begin(), 2));
	
	return std::vector<unsigned int>{result.begin(), result.end()};
}