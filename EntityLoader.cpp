#include "EntityLoader.h"
#include "ecs_EntityManager.h"
#include "SimpleTextProcessor.h"

#include "Transform.h"
#include "RenderMesh.h"
#include "Camera.h"
#include "CameraController.h"

void LoadComponentTransform(int a, std::map<std::string, int>& names, std::map<std::string, std::string>& res) 
{
	Transform& tr = ECS::DefEcs_().entity.GetComponent<Transform>(a);
	tr.Position = TextTools::ReadVec3(res["Position"]);
	tr.Scale = TextTools::ReadVec3(res["Scale"]);
	//tr.Rotation = TextTools::ReadVec3(res["Rotation"]);
}

void LoadComponentRenderMesh(int a, std::map<std::string, int>& names, std::map<std::string, std::string>& res)
{
	Singleton<SharedGraphicsResources> singlRes;
	RenderMesh& renMesh = ECS::DefEcs_().entity.GetComponent<RenderMesh>(a);
	renMesh.RenderedMesh = GLMesh(singlRes->ModelCont.GetModelRef(res["RenderMesh"]).Meshes[0]);
	renMesh.RenderMaterial = GLMaterial(singlRes->GetMaterial(res["RenderMaterial"]));
}

void LoadComponentCamera(int a, std::map<std::string, int>& names, std::map<std::string, std::string>& res)
{
	Camera& cam = ECS::DefEcs_().entity.GetComponent<Camera>(a);
	cam.FOV = glm::radians(std::stof(res["FOV"]));
	cam.NearClip = std::stof(res["NearClip"]);
	cam.FarClip = std::stof(res["FarClip"]);
}

void LoadComponentCameraController(int a, std::map<std::string, int>& names, std::map<std::string, std::string>& res)
{
	CameraController& contr = ECS::DefEcs_().entity.GetComponent<CameraController>(a);
	contr.MouseSensitivity = std::stof(res["MouseSensitivity"]);
	contr.Speed = std::stof(res["Speed"]);
}

void SerializationSystem::LoadEntity(std::filesystem::path path)
{
	static std::map<std::string, std::pair<void(ECS::EntityManager::*)(int), void(*)(int, std::map<std::string, int>&, std::map<std::string, std::string>&)>> links
	{
		{"Transform", {&ECS::EntityManager::AddComponent<Transform>, LoadComponentTransform}},
		{"RenderMesh", {&ECS::EntityManager::AddComponent<RenderMesh>, LoadComponentRenderMesh}},
		{"Camera", {&ECS::EntityManager::AddComponent<Camera>, LoadComponentCamera}},
		{"CameraController", {&ECS::EntityManager::AddComponent<CameraController>, LoadComponentCameraController}}
	};
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
			int a = ECS::DefEcs_().entity.AddEntity<>();
			if (!name.empty())
			{
				names[name] = a;
			}
			//загрузка компонентов
			if (str == "{")
			{
				file >> str;
				
				while (str != "}")
				{
					std::string nameComp = str;
					(ECS::DefEcs_().entity.*links[str].first)(a);

					file >> str;
					if (str == "{")
					{
						std::map<std::string, std::string> map;
						file >> str;
						while (str != "}")
						{
							std::string val;
							file >> val;
							std::getline(file, val);
							map[str] = TextTools::DelChar(val, ' ');
							file >> str;
						}
						loadedComp[{a, nameComp}] = std::move(map);
						file >> str;
					}
				}
			}
		}
	}

	for (auto l : loadedComp)
	{
		(links[l.first.second].second)(l.first.first, names, l.second);
	}
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