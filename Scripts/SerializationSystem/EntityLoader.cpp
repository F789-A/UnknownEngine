#include "EntityLoader.h"

#include <vector>
#include <string>
#include <fstream>

#include "ECS\EcsSystem.h"

#include "Utils\SimpleTextProcessor.h"
#include "Logger.h"

#include "Core\Input.h"

#include "StringToGLFWKey.h"

namespace
{
	std::set<char> sep = {' ', '\n', '\t', '=', ',', '[', ']', '{', '}', '/', '"', '"' };
	std::set<char> ignor = {' ', '\n', '\t'};

	void SkipLine(std::ifstream& inp)
	{
		char ch = inp.get();
		while (!inp.eof() && ch != '\n')
		{
			ch = inp.get();
		}
	}

	void CheckCorrect(const std::string& str, const std::string& cor)
	{
		if (str != cor)
		{
			throw "bad";
		}
	}

	void CheckTerminal(const std::string& str)
	{
		if (str.size() == 0 || ( str.size() == 1 && sep.contains(str[0]))) {
			throw "bad";
		}
	}

	std::string ReadPreToken(std::ifstream& inp)
	{
		char ch = inp.get();
		std::string res;
		bool skipIgnor = true;
		while (skipIgnor)
		{
			skipIgnor = false;
			while (!inp.eof() && ignor.contains(ch)) {
				ch = inp.get();
			}
			if (inp.eof()) {
				return "";
			}
			if (ch == '/'){
				ch = inp.get();
				if (ch == '/') {
					SkipLine(inp);
					skipIgnor = true;
					ch = inp.get();
				}
				else{
					inp.seekg(-1, std::ios::cur);
				}
			}
		}
		if (sep.contains(ch)){
			return { ch };
		}
		while (!sep.contains(ch)){
		    res.push_back(ch);
			ch = inp.get();
		}
		if (!ignor.contains(ch)) {
			inp.seekg(-1, std::ios::cur);
		}
		return res;
	}

	std::string ReadString(std::ifstream& inp)
	{
		char ch = inp.get();
		CheckCorrect({ ch }, "\"");
		ch = inp.get();
		std::string result;
		while (ch != '"')
		{
			result.push_back(ch);
			ch = inp.get();
		}
		return result;
	}
}

void SerializationSystem::LoadEntity(ecs::EntityManager& em, std::filesystem::path path)
{
	std::map<std::pair<int, std::string>, std::map<std::string, std::string>> loadedComp;
	std::map<std::string, int> names;
	Singleton<Logger> logger;
	std::ifstream file(path);
	if (!file){
		logger->Log("File of entity doesn't open");
		throw std::exception("File of entity doesn't open");
	}

	while (!file.eof())
	{
		std::string str = ReadPreToken(file);
		if (str == "")
		{
			continue;
		}
		if (str == "Entity")
		{
			std::string name;
			int ent;
			str = ReadPreToken(file);
			if (str == "as")
			{
				str = ReadPreToken(file);
				CheckTerminal(str);
				if (names.contains(str)) {
					ent = names[str];
				}
				else {
					ent = em.AddEntity<>();
				}
				str = ReadPreToken(file);
			}
			else {
				ent = em.AddEntity<>();
			}
			CheckCorrect(str, "{");
			str = ReadPreToken(file);
			while (str != "}")
			{
			
				CheckTerminal(str);
				std::string compName = str;
				ecs::CreateComponent(em, compName, ent); // Создаем компонент
				str = ReadPreToken(file);
				CheckCorrect(str, "{");
				str = ReadPreToken(file);
				std::map<std::string, std::string> compArgs;
				while (str != "}")
				{
					bool isArray = false;
					bool isRef = false;
					CheckTerminal(str);
					//обработка типа
					if (str == "ref") {
						isRef = true;
					}
					str = ReadPreToken(file);
					if (str == "[")
					{
						str = ReadPreToken(file);
						CheckCorrect(str, "]");
						isArray = true;
						str = ReadPreToken(file);
					}
					CheckTerminal(str);
					std::string paramName = str;
					str = ReadPreToken(file);
					CheckCorrect(str, "=");
					std::string paramValue;
					bool firstPass = true;

					if (isArray)
					{
						str = ReadPreToken(file);
						CheckCorrect(str, "[");
					}
					str = ReadPreToken(file);
					while ((firstPass || (isArray && str != "]")) && !(isArray && str == "]"))
					{
						if (!firstPass)
						{
							paramValue += ", ";
							CheckCorrect(str, ",");
							str = ReadPreToken(file);
						}
						firstPass = false;
						if (isRef)
						{
							CheckTerminal(str);
							if (!names.contains(str))
							{
								int newEnt = em.AddEntity<>();
								names[str] = newEnt;
							}
							paramValue += std::to_string(names[str]);
						}
						else if (str == "\"")
						{
							file.seekg(-1, std::ios::cur);
							paramValue += ReadString(file);
						}
						else {
							paramValue += str;
						}
						if (isArray)
						{
							str = ReadPreToken(file);
						}
					}
					compArgs[paramName] = paramValue;
					str = ReadPreToken(file);
				}
				loadedComp[{ent, compName}] = std::move(compArgs);
				str = ReadPreToken(file);
			}
		}
		else
		{
			throw "bad";
		}
	}
	for (auto l : loadedComp)
	{
		ecs::LoadComponent(l.first.second, em, l.first.first, l.second);
	}
}


void SerializationSystem::LoadKeyFromFile(Input& inp, std::filesystem::path path)
{
	std::fstream file = std::fstream(path, std::ios_base::in);
	if (!file)
	{
		throw std::exception("Key file doesn't open");
	}

	std::string inStr;
	while (std::getline(file, inStr))
	{
		std::vector<std::string> param = TextTools::SplitAndDelSpace(inStr, '=');
		inp.BindKey(param[0], StringToGLFWKey.at(param[1]));
	}
}