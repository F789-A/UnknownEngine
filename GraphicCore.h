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

	Skybox SkyboxThat;
	std::unique_ptr<PostProcessing> PostProcess;

	GraphicCore();
	GraphicCore operator=(const GraphicCore&) = delete;
	GraphicCore(const GraphicCore&) = delete;
	GraphicCore(GraphicCore&& other) = delete;
	GraphicCore& operator=(GraphicCore&& other) = delete;

	

	~GraphicCore() = default;
	unsigned int uniformCameraBlock;

	GLCubemapTexture& GetSkyboxTexture();
	GLShader& GetSkyboxShader();
public:
	bool EnablePostProcessing = false;

	static GraphicCore& GetInstance();
	void UpdateGraphic();
};
