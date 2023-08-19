#pragma once
#include <glad/glad.h> 
#include <GLFW\glfw3.h>
#include "WindowApp.h"
#include <vector>
#include <glm/gtc/type_ptr.hpp>

#include "Singleton.h"
#include "SharedGraphicsResources.h"
#include "GlFrameBuffer.h"

#include <functional>

using uint = unsigned int;

class GraphicCore
{
	GLFWwindow* Window;

	std::vector<GLMaterial> PostProcesses;

	GlFramebuffer uiFramebuffer;
	GlFramebuffer sceneFramebuffer;
	GlFramebuffer postProcessFramebuffer;
	GLMaterial BlendSceneMaterial;

	uint uniformCameraBlock;

	uint Uniform_Shaders_Parameters;

	GraphicCore(GLFWwindow* window);
	GraphicCore operator=(const GraphicCore&) = delete;
	GraphicCore(const GraphicCore&) = delete;
	GraphicCore(GraphicCore&& other) = delete;
	GraphicCore& operator=(GraphicCore&& other) = delete;

	~GraphicCore() = default;

public:
	bool EnablePostProcessing = false;
	int Height;
	int Width;
	float Aspect = 0.75f;
	std::function<void()> mainPassFunc;
	std::function<void()> uiPassFunc;
	std::function<std::pair<glm::mat4, glm::mat4>()> GetCameraMatrices; // <projection, view> matrices from camera
	std::function<std::vector<GLMaterial>()> GetPostProcesses;

public:
	static GraphicCore& GetInstance();
	void UpdateGraphic();
};
