#pragma once
#include <glad/glad.h> 
#include <GLFW\glfw3.h>
#include "WindowApp.h"
#include <vector>
#include "ILoopUpdate.h"
#include <glm/gtc/type_ptr.hpp>

#include "LegacyCamera.h"
#include "ICamera.h"
#include "Singleton.h"
#include "Skybox.h"
#include "SharedGraphicsResources.h"
#include "PostProcessing.h"

class GraphicCore
{
	std::unique_ptr<Skybox> CurrentSkybox;
	std::unique_ptr<PostProcessing> PostProcess;
	unsigned int uniformCameraBlock;

	GraphicCore();
	GraphicCore operator=(const GraphicCore&) = delete;
	GraphicCore(const GraphicCore&) = delete;
	GraphicCore(GraphicCore&& other) = delete;
	GraphicCore& operator=(GraphicCore&& other) = delete;

	~GraphicCore() = default;

public:
	bool EnablePostProcessing = false;

	static GraphicCore& GetInstance();
	void UpdateGraphic();
};
