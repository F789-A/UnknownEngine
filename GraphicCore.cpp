#include "GraphicCore.h"

GraphicCore::GraphicCore()
{
	//init advice
	Singleton<SharedGraphicsResources> SinglRes;
	GLShader& shad = SinglRes->GetShaderRef("Shaders/Pixelization.ueshad");
	PostProcess = std::make_unique<PostProcessing>(shad);
	shad.SetInt("aspectX", WindowApp::GetInstance().Width()); // TODO вариационный шаблон для установления параметров по умолчанию для PC
	shad.SetInt("aspectY", WindowApp::GetInstance().Height());
	shad.SetInt("intencity", 100);

	GLCubemapTexture& skMap = SinglRes->GetGLCubemapRef("skybox/DefaultSkybox.ueskybox");
	GLShader& skShad = SinglRes->GetShaderRef("Shaders/Skybox.ueshad");
	CurrentSkybox = std::make_unique<Skybox>(skMap, skShad);

	//init GL
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glGenBuffers(1, &uniformCameraBlock);
	glBindBuffer(GL_UNIFORM_BUFFER, uniformCameraBlock);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, uniformCameraBlock);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glfwSetInputMode(WindowApp::GetInstance().GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

GraphicCore& GraphicCore::GetInstance()
{
	static GraphicCore  instance;
	return instance;
}

void GraphicCore::UpdateGraphic()
{
	if (EnablePostProcessing)
	{
		PostProcess->Use();
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 view = ICamera::MainCamera->GetViewMatrix();
	glm::mat4 projection = glm::perspective(ICamera::MainCamera->GetFOV(), (float)WindowApp::GetInstance().Width() / WindowApp::GetInstance().Height(), 0.1f, 100.0f);
	glBindBuffer(GL_UNIFORM_BUFFER, uniformCameraBlock);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	
	for (int i = 0; i < ILoopUpdate<UpdateType::GraphicLoop>::UpdateVector.size(); i++)
	{
		ILoopUpdate<UpdateType::GraphicLoop>::UpdateVector[i]->Update();
	}

	if (CurrentSkybox.get() != nullptr)
	{
		CurrentSkybox->Draw(view, projection);
	}

	if (EnablePostProcessing)
	{
		PostProcess->Render();
	}

	glfwSwapBuffers(WindowApp::GetInstance().GetWindow());
}