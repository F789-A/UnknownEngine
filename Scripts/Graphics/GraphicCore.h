#pragma once
#include <glad/glad.h> 
#include <GLFW\glfw3.h>

#include <vector>
#include <functional>

#include <glm/gtc/type_ptr.hpp>

#include "Utils\Singleton.h"
#include "GlFrameBuffer.h"
#include "GLMaterial.h"

using uint = unsigned int;

class GraphicCore
{
private:
	GLFWwindow* Window;

	GlFramebuffer uiFramebuffer;
	GlFramebuffer sceneFramebuffer;
	GlFramebuffer postProcessFramebuffer;
	GLMaterial BlendSceneMaterial;

	GLuint uniformCameraBlock;
	GLuint uniformShadersParameters;

	GraphicCore(GLFWwindow* window, int width, int height);
	GraphicCore operator=(const GraphicCore&) = delete;
	GraphicCore(const GraphicCore&) = delete;
	GraphicCore(GraphicCore&& other) = delete;
	GraphicCore& operator=(GraphicCore&& other) = delete;

	~GraphicCore() = default;

public:
	int Height;
	int Width;
	std::function<void()> mainPassFunc;
	std::function<void()> uiPassFunc;
	std::function<std::pair<glm::mat4, glm::mat4>()> GetCameraMatrices; // <projection, view> matrices from camera
	std::function<std::vector<GLMaterial*>()> GetPostProcesses;

public:
	static GraphicCore& GetInstance();
	void UpdateGraphic();
	float GetAspect() const;
};
