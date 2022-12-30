#pragma once
#include <glad/glad.h> 
#include <GLFW\glfw3.h>
#include "WindowApp.h"
#include <vector>
#include <glm/gtc/type_ptr.hpp>



#include "Singleton.h"
#include "Skybox.h"
#include "SharedGraphicsResources.h"
#include "PostProcessing.h"

using uint = unsigned int;

class GraphicCore
{
	std::unique_ptr<Skybox> CurrentSkybox;
	std::unique_ptr<PostProcessing> PostProcess;

	std::vector<PostProcessing> _PostProcess;

	uint uniformCameraBlock;

	uint Uniform_Shaders_Parameters;

	GraphicCore();
	GraphicCore operator=(const GraphicCore&) = delete;
	GraphicCore(const GraphicCore&) = delete;
	GraphicCore(GraphicCore&& other) = delete;
	GraphicCore& operator=(GraphicCore&& other) = delete;

	~GraphicCore() = default;

public:
	bool EnablePostProcessing = false;
	int Height;
	int Weigth;

	std::vector<void(*)()> mainPass;
	std::vector<void(*)()> UiPass;
	
	static GraphicCore& GetInstance();
	void UpdateGraphic();
};
