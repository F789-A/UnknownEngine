#include "GraphicCore.h"
#include "UIHandler.h"

GraphicCore::GraphicCore()
{
	//init advice
	Singleton<SharedGraphicsResources> SinglRes;
	GLShader& shad = SinglRes->GetShaderRef("Shaders/Pixelization.ueshad");
	PostProcess = std::make_unique<PostProcessing>(shad);
	shad.SetInt("intencity", 100);

	GLCubemapTexture& skMap = SinglRes->GetGLCubemapRef("skybox/DefaultSkybox.ueskybox");
	GLShader& skShad = SinglRes->GetShaderRef("Shaders/Skybox.ueshad");
	CurrentSkybox = std::make_unique<Skybox>(skMap, skShad);

	//init GL
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//
	glGenBuffers(1, &uniformCameraBlock);
	glBindBuffer(GL_UNIFORM_BUFFER, uniformCameraBlock);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, uniformCameraBlock);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	
	//
	int aspect[2] {WindowApp::GetInstance().Width(), WindowApp::GetInstance().Height()};
	glGenBuffers(1, &Uniform_Shaders_Parameters);
	glBindBuffer(GL_UNIFORM_BUFFER, Uniform_Shaders_Parameters);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(GLuint), NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 3, Uniform_Shaders_Parameters);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 2*sizeof(GLuint), aspect);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//glfwSetInputMode(WindowApp::GetInstance().GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

	for (int i = 0; i < mainPass.size(); i++)
	{
		mainPass[i]();
	}

	for (auto& uiFunc : UiPass)
	{
		uiFunc();
	}

	if (CurrentSkybox != nullptr)
	{
		CurrentSkybox->Draw(view, projection);
	}

	if (EnablePostProcessing)
	{
		PostProcess->Render();
	}

	glfwSwapBuffers(WindowApp::GetInstance().GetWindow());
}