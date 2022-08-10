#include "EntityLoader.h"

void EntityLoader::Load()
{
	std::regex rgxAddComp("(AddComponent<)([^>]+)(?=>)"); // ретроспективной проверки нет(
	std::regex rgxAddEntity("^AddEntity$");
	SimpleTextProcerssor textTool;
	auto current_end = std::sregex_iterator();

	ec::Entity* currentEntity = nullptr;

	Singleton<SharedGraphicsResources> SinglRes;
	Singleton<ec::EntityManager> singlEntityManager;
	std::ifstream file(Path.c_str());

	if (!file)
	{
		throw std::exception("Не получилось открыть файл сущностей");
	}

	std::string str;
	std::getline(file, str);
	while (file)
	{
		if (std::regex_match(str, rgxAddEntity))
		{
			currentEntity = &(singlEntityManager->AddEntity());
			
		}
		else if (std::regex_search(str, rgxAddComp))
		{
			std::sregex_iterator current(str.begin(), str.end(), rgxAddComp);
			std::smatch match = *current;
			std::string componentName = match.str();
			componentName = componentName.substr(13, componentName.size() - 13);

			Singleton<Logger> logger;

			if (componentName == "Transform")
			{
				Transform& transf = currentEntity->AddComponent<Transform>();

				std::string paramRow = textTool.GetArea(str, "()");
				std::vector<std::string> param = textTool.SplitAndDelSpace(paramRow, ',');
				transf.Position = glm::vec3(std::atoi(param[0].c_str()), std::atoi(param[1].c_str()), std::atoi(param[2].c_str()));
			}
			if (componentName == "ComponentThatAlwaysSayHello")
			{
				currentEntity->AddComponent<ComponentThatAlwaysSayHello>();
			}
			if (componentName == "RenderMesh")
			{

				RenderMesh& renMesh = currentEntity->AddComponent<RenderMesh>();

				std::string paramRow = textTool.GetArea(str, "()");
				std::vector<std::string> param = textTool.SplitAndDelSpace(paramRow, ',');

				renMesh.RenderedMesh = GLMesh((SinglRes->ModelCont.GetModelRef(param[0]).Meshes[0]));
				renMesh.RenderMaterial = SinglRes->GetMaterial(param[1]);
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
				currentEntity->AddComponent<EscapeHandler>();
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

void EntityLoader::LoadKeyFromFile(std::filesystem::path path)
{
	std::fstream file = std::fstream(path, std::ios_base::in);
	std::string inStr;
	SimpleTextProcerssor textTool;
	while (std::getline(file, inStr))
	{
		std::vector<std::string> param = textTool.SplitAndDelSpace(inStr, '=');
		Input::GetInstance().BindKey(param[0], StringToGLFWKey.at(param[1]));
	}
}