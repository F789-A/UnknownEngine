#pragma once
#include <glad/glad.h> 
#include <GLFW\glfw3.h>
#include "EngineCore/WindowApp.h"
#include <vector>
#include "ILoopUpdate.h"
#include <glm/gtc/type_ptr.hpp>

#include "LegacyCamera.h"
#include "ICamera.h"
#include "Singleton.h"
#include "Skybox.h"
#include "SharedGraphicsResources.h"

class GraphicCore
{
	GraphicCore();
	GraphicCore operator=(const GraphicCore&) = delete;
	GraphicCore(const GraphicCore&) = delete;
	GraphicCore(GraphicCore&& other) = delete;
	GraphicCore& operator=(GraphicCore&& other) = delete;

	Skybox SkyboxThat;

	~GraphicCore() = default;
	unsigned int uniformCameraBlock;

	GLCubemapTexture& GetSkyboxTexture();
	GLShader& GetSkyboxShader();
public:
	static GraphicCore& GetInstance();

	void UpdateGraphic();
};
