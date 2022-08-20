#include "EntityLoader.h"

void EntityLoader::Load()
{
	SimpleTextProcessor textTool;
	Singleton<SharedGraphicsResources> singlRes;
	Singleton<ec::EntityManager> singlEntityManager;
	Singleton<Logger> logger;

	ec::Entity* currentEntity = nullptr;

	std::ifstream file(Path.c_str());
	if (!file)
	{
		throw std::exception("File of entity doesn't open");
	}

	std::string str;
	while (std::getline(file, str))
	{
		if (str == "AddEntity")
		{
			currentEntity = &(singlEntityManager->AddEntity());
		}
		else if (str.find("AddComponent"))
		{
			std::string componentName = textTool.GetArea(str, "<>");

			if (componentName == "Transform")
			{
				std::string paramRow = textTool.GetArea(str, "()");
				std::vector<std::string> param = textTool.SplitAndDelSpace(paramRow, ',');
				glm::vec3 pos(std::atoi(param[0].c_str()), std::atoi(param[1].c_str()), std::atoi(param[2].c_str()));
				currentEntity->AddComponent<Transform>(pos);
			}
			if (componentName == "ComponentThatAlwaysSayHello")
			{
				currentEntity->AddComponent<ComponentThatAlwaysSayHello>(std::string("I am fucking born!!!"));
			}
			if (componentName == "RenderMesh")
			{
				std::string paramRow = textTool.GetArea(str, "()");
				std::vector<std::string> param = textTool.SplitAndDelSpace(paramRow, ',');

				currentEntity->AddComponent<RenderMesh>(GLMesh((singlRes->ModelCont.GetModelRef(param[0]).Meshes[0])),
					singlRes->GetMaterial(param[1]));
			}
			else if (componentName == "Camera")
			{
				Camera& camer = currentEntity->AddComponent<Camera>();
				ICamera::MainCamera = &camer;
			}
			else if (componentName == "CameraController")
			{
				currentEntity->AddComponent<CameraController>();
			}
			else if (componentName == "EscapeHandler")
			{
				currentEntity->AddComponent<InputHandler>();
			}
		}
		else
		{
			throw std::exception("Invalid data of entity");
		}
	}
}

void EntityLoader::LoadKeyFromFile(std::filesystem::path path)
{
	SimpleTextProcessor textTool;

	std::fstream file = std::fstream(path, std::ios_base::in);
	if (!file)
	{
		throw std::exception("Key file doesn't open");
	}

	std::string inStr;
	while (std::getline(file, inStr))
	{
		std::vector<std::string> param = textTool.SplitAndDelSpace(inStr, '=');
		Input::GetInstance().BindKey(param[0], StringToGLFWKey.at(param[1]));
	}
}