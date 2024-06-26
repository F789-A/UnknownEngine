#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

//��������� ������ std::regex
namespace TextTools
{
	std::vector<std::string> SplitAndDelSpace(const std::string& strIn, const char sep);

	std::string DelChar(const std::string& strIn, char ch);
	glm::vec3 ReadVec3(const std::string& str);
	glm::vec2 ReadVec2(const std::string& str);
	std::vector<int> ReadIntArray(const std::string& str);
	std::vector<std::string> ReadStringArray(const std::string& str);
	bool ReadBool(const std::string& str);
}