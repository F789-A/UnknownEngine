#include "EntityLoader.h"
#include "EcsSystem.h"
#include "SimpleTextProcessor.h"
#include "Logger.h"
#include <fstream>
#include <string>
#include <vector>

#include "StringToGLFWKey.h"

void SerializationSystem::LoadEntity(ecs::EntityManager& em, std::filesystem::path path)
{
	std::map<std::pair<int, std::string>, std::map<std::string, std::string>> loadedComp;
	std::map<std::string, int> names;
	Singleton<Logger> logger;
	std::ifstream file(path);
	if (!file)
	{
		logger->Log("File of entity doesn't open");
		throw std::exception("File of entity doesn't open");
	}

	std::string str;
	while (file >> str)
	{
		if (str == "Entity")
		{
			std::string name;
			file >> str;
			if (str == "=")
			{
				file >> name;
				file >> str;
			}
			int ent;
			if (!name.empty() && names.find(name) != names.end())
			{
				ent = names[name];
			}
			else
			{
				ent = em.AddEntity<>();
				if (!name.empty() && names.find(name) == names.end())
				{
					names[name] = ent;
				}
			}
			//загрузка компонентов
			if (str == "{")
			{
				file >> str;

				while (str != "}")
				{
					std::string nameComp = str;
					(em.*ecs::LoadCallbacks()[str].first)(ent);

					file >> str;
					if (str == "{")
					{
						std::map<std::string, std::string> compArgs;
						file >> str;
						while (str != "}")
						{
							if (str == "component")
							{
								std::string param;
								file >> param;
								std::string val;
								//file >> val >> val;
								std::getline(file, val);
								val = TextTools::DelChar(val, ' ');
								val = val.substr(1, val.size() - 1);
								std::vector<std::string> vals = TextTools::SplitAndDelSpace(val, ',');
								compArgs[param] = "";
								for (auto l : vals)
								{
									if (names.find(l) != names.end())
									{
										if (compArgs[param].empty())
										{
											compArgs[param] += std::to_string(names[l]);
										}
										else
										{
											compArgs[param] += ", " + std::to_string(names[l]);
										}
									}
									else
									{
										int newEnt = em.AddEntity<>();
										if (compArgs[param].empty())
										{
											compArgs[param] += std::to_string(newEnt);
										}
										else
										{
											compArgs[param] += ", " + std::to_string(newEnt);
										}
										names[l] = newEnt;
									}
								}
							}
							else
							{
								std::string val;
								file >> val;
								std::getline(file, val);
								compArgs[str] = TextTools::DelChar(val, ' ');
							}
							file >> str;
						}
						loadedComp[{ent, nameComp}] = std::move(compArgs);
						file >> str;
					}
				}
			}
		}
	}
	for (auto l : loadedComp)
	{
		(ecs::LoadCallbacks()[l.first.second].second)(em, l.first.first, l.second);
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