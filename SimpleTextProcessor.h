#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

//использую взамен std::regex
namespace TextTools
{
	std::vector<std::string> SplitAndDelSpace(const std::string& strIn, const char sep);

	std::string GetArea(const std::string& strIn, const std::string& strTempl);

	glm::vec3 ReadVec3(const std::string& str);

	std::vector<int> GetVectorInt(const std::string& str);
}