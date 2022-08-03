#include "EntityLoader.h"

std::vector<std::string> EntityLoader::Split(const std::string& str)
{
	std::vector<std::string> result;
	std::regex rgxFindArg("[^,]+");

	std::sregex_iterator _current(str.begin(), str.end(), rgxFindArg);
	for (auto it = _current; it != std::sregex_iterator(); it++)
	{
		std::smatch match = *it;
		std::string _str = match.str();
		if (_str[0] == ' ')
		{
			_str.erase(0, 1);
		}
		result.push_back(_str);
	}
	return result;
}

void EntityLoader::Load()
{
	std::regex rgxAddComp("(AddComponent<)([^>]+)(?=>)"); // ��������������� �������� ���(
	std::regex rgxAddEntity("^AddEntity$");
	std::regex rgxFindArg("\"([^\"]+)(?=\")");
	std::regex rgxFindPar("([\(]([^\)]+))");
	SimpleTextProcerssor textTool;
	auto current_end = std::sregex_iterator();

	Entity currentEntity = -1;

	Singleton<SharedGraphicsResources> SinglRes;

	std::ifstream file(Path.c_str());

	if (!file)
	{
		throw std::exception("�� ���������� ������� ���� ���������");
	}

	std::string str;
	std::getline(file, str);
	while (file)
	{
		if (std::regex_match(str, rgxAddEntity))
		{
			currentEntity = EntityManager::Manager().CreateEntity();
		}
		else if (std::regex_search(str, rgxAddComp))
		{
			std::sregex_iterator current(str.begin(), str.end(), rgxAddComp);
			std::smatch match = *current;
			std::string componentName = match.str();
			componentName = componentName.substr(13, componentName.size() - 13);

			if (componentName == "Transform")
			{
				EntityManager::Manager().AddComponent<Transform>(currentEntity);
				Transform& transf = EntityManager::Manager().GetComponent<Transform>(currentEntity);

				std::string paramRow = textTool.GetArea(str, "()");
				std::vector<std::string> param = textTool.SplitAndDelSpace(paramRow, ',');
				transf.Position = glm::vec3(std::atoi(param[0].c_str()), std::atoi(param[1].c_str()), std::atoi(param[2].c_str()));
			}
			if (componentName == "ComponentThatAlwaysSayHello")
			{
				EntityManager::Manager().AddComponent<ComponentThatAlwaysSayHello>(currentEntity);
			}
			if (componentName == "RenderMesh")
			{
				EntityManager::Manager().AddComponent<RenderMesh>(currentEntity);
				RenderMesh& renMesh = EntityManager::Manager().GetComponent<RenderMesh>(currentEntity);

				std::string paramRow = textTool.GetArea(str, "()");
				std::vector<std::string> param = textTool.SplitAndDelSpace(paramRow, ',');

				renMesh.RenderedMesh = GLMesh((SinglRes->ModelCont.GetModelRef(param[0]).Meshes[0]));
				renMesh.RenderMaterial = SinglRes->GetMaterial(param[1]);
			}
			else if (componentName == "Camera")
			{
				EntityManager::Manager().AddComponent<Camera>(currentEntity);
				Camera* camer = &EntityManager::Manager().GetComponent<Camera>(currentEntity);
				ICamera::MainCamera = camer;

			}
			else if (componentName == "CameraController")
			{
				EntityManager::Manager().AddComponent<CameraController>(currentEntity);
			}
		}
		else
		{
			std::cout << "Invalid data" << std::endl;
			throw "Invalid data";
		}
		std::getline(file, str);
	}
}