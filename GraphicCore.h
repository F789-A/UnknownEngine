#pragma once
#include <glad/glad.h> 
#include <GLFW\glfw3.h>
#include "WindowApp.h"
#include <vector>
#include <glm/gtc/type_ptr.hpp>

#include "Singleton.h"
#include "Skybox.h"
#include "SharedGraphicsResources.h"
#include "GlFrameBuffer.h"
#include "ecs_EntityManager.h"

using uint = unsigned int;

class GraphicCore
{
	std::unique_ptr<Skybox> CurrentSkybox;

	std::vector<GLMaterial> PostProcesses;

	GlFramebuffer uiFramebuffer;
	GlFramebuffer sceneFramebuffer;
	GlFramebuffer postProcessFramebuffer;
	GLMaterial BlendSceneMaterial;

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
	int Width;
	ecs::EcsSystem* ecsS;

	std::vector<void(*)(ecs::EntityManager&)> mainPass;
	std::vector<void(*)(ecs::EntityManager&)> UiPass;
	
	static GraphicCore& GetInstance();
	void UpdateGraphic();
};
