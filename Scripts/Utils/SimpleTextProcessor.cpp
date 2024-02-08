#include "SimpleTextProcessor.h"

std::string TextTools::DelChar(const std::string& strIn, char ch)
{
	std::string res;
	for (auto st : strIn)
	{
		if (st != ch)
		{
			res.push_back(st);
		}
	}
	return res;
}

std::string TextTools::TrimStart(const std::string& str, int num)
{
	return str.substr(num, str.length());
}

std::vector<std::string> TextTools::SplitAndDelSpace(const std::string& strIn, const char sep)
{
	std::vector<std::string> res;
	std::string str = "";
	for (auto st : strIn)
	{
		if (st == sep)
		{
			res.push_back(str);
			str = "";
		}
		else if (st != ' ')
		{
			str.push_back(st);
		}
	}
	if (!str.empty())
	{
		res.push_back(str);
	}
	return res;
}

std::string TextTools::GetArea(const std::string& strIn, const std::string& strTempl)
{
	int indSt = strIn.find(strTempl[0]) + 1;
	int indEnd = strIn.find(strTempl[1]) - 1;
	return strIn.substr(indSt, indEnd - indSt + 1);
}

glm::vec3 TextTools::ReadVec3(const std::string& str)
{
	std::string _str = DelChar(str, ' ');
	//_str = _str.substr(1, _str.length()-2);
	std::vector<std::string> coords = TextTools::SplitAndDelSpace(_str, ',');
	return glm::vec3(std::stof(coords[0]), std::stof(coords[1]), std::stof(coords[2]));
}

std::vector<int> TextTools::ReadIntArray(const std::string& str)
{
	std::string _str = DelChar(str, ' ');
	std::vector<std::string> vals = TextTools::SplitAndDelSpace(_str, ',');
	std::vector<int> result;
	vals.reserve(vals.size());
	for (auto& l : vals)
	{
		result.push_back(std::stoi(l));
	}
	return result;
}

bool TextTools::ReadBool(const std::string& str)
{
	if (str == "true")
	{
		return true;
	}
	if (str == "false")
	{
		return false;
	}
	throw "Bad arg";
}

std::vector<int> TextTools::GetVectorInt(const std::string& str)
{
	std::string _str = str.substr(1, str.length() - 2);
	std::vector<std::string> coords = TextTools::SplitAndDelSpace(_str, ',');
	std::vector<int> result;
	for (auto& l : coords)
	{
		result.push_back(std::stoi(l));
	}
	return result;
}