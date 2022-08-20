#pragma once

#include <string>
#include <vector>

//использую взамен std::regex
class SimpleTextProcessor
{
private:
	
public:
	std::vector<std::string> SplitAndDelSpace(const std::string& strIn, const char sep)
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
		res.push_back(str);
		return res;
	}

	std::string GetArea(const std::string& strIn, const std::string& strTempl)
	{
		int indSt = strIn.find(strTempl[0])+1;
		int indEnd = strIn.find(strTempl[1])-1;
		return strIn.substr(indSt, indEnd - indSt + 1);
	}
};