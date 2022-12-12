#include "EntityLoader.h"

void SerializationSystem::LoadEntity(std::filesystem::path path)
{
	std::vector<std::tuple<int, std::string, std::string, int, std::string>> linkedComponents;
	Singleton<SharedGraphicsResources> singlRes;
	Singleton<ec::EntityManager> singlEntityManager;
	Singleton<Logger> logger;

	ec::Entity* currentEntity = nullptr;

	std::ifstream file(path);
	if (!file)
	{
		logger->Log("File of entity doesn't open");
		throw std::exception("File of entity doesn't open");
	}
	
	std::string str;

	while (std::getline(file, str))
	{
		if (str.find("Entity") != std::string::npos)
		{
			currentEntity = &(singlEntityManager->AddEntity());
		}
		else if (str.find("Transform") != std::string::npos)
		{
				Transform& tr = currentEntity->AddComponent<Transform>();
				std::getline(file, str);
				tr.setPosition(TextTools::ReadVec3(str));
				std::getline(file, str);
				tr.Rotation = TextTools::ReadVec3(str);
				std::getline(file, str);
				tr.Scale = TextTools::ReadVec3(str);
				std::getline(file, str);
				tr.isLocal = std::stoi(str);
		}
		else if (str.find("ComponentThatAlwaysSayHello") != std::string::npos)
		{
			currentEntity->AddComponent<ComponentThatAlwaysSayHello>(std::string("I am born!!!"));
		}
		else if (str.find("RenderMesh") != std::string::npos)
		{
			std::vector<std::string> param(2);
			std::getline(file, param[0]);
			std::getline(file, param[1]);
			currentEntity->AddComponent<RenderMesh>(GLMesh((singlRes->ModelCont.GetModelRef(param[0]).Meshes[0])),
				singlRes->GetMaterial(param[1]));
		}
		else if (str == "Camera")
		{
			Camera& camer = currentEntity->AddComponent<Camera>();
			ICamera::MainCamera = &camer;
		}
		else if (str.find("CameraController") != std::string::npos)
		{
			currentEntity->AddComponent<CameraController>();
		}
		else if (str.find("EscapeHandler") != std::string::npos)
		{
			currentEntity->AddComponent<InputHandler>();
		}
		else if (str.find("CarController") != std::string::npos)
		{
			currentEntity->AddComponent<CarController>();
		}
		else if (str.find("Link") != std::string::npos)
		{
			std::getline(file, str);
			currentEntity = &(singlEntityManager->GetEntity(std::stoi(str)));
			std::getline(file, str);
			if (str.find("Transform") != std::string::npos)
			{
				Transform& tr = currentEntity->GetComponent<Transform>();
				std::getline(file, str);
				if (std::stoi(str) == -1)
				{
					tr.parent = nullptr;
				}
				else
				{
					tr.parent = &(singlEntityManager->GetEntity(std::stoi(str)).GetComponent<Transform>());
				}
				std::getline(file, str);
				std::vector<int> ch = TextTools::GetVectorInt(str);
				for (auto l : ch)
				{
					if (l != -1)
					{
						tr.childs.push_back(&(singlEntityManager->GetEntity(l).GetComponent<Transform>()));
					}
				}
			}
		}
		else
		{
			throw std::exception("Invalid data of entity");
		}
	}
}

void LoadEntity_(std::filesystem::path path)
{

}

void SerializationSystem::LoadKeyFromFile(std::filesystem::path path)
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
		Input::GetInstance().BindKey(param[0], StringToGLFWKey.at(param[1]));
	}
}