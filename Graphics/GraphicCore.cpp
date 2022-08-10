#include "Graphics/GraphicCore.h"

GraphicCore::GraphicCore(): SkyboxThat(GetSkyboxTexture(), GetSkyboxShader())
{
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

std::vector<Texture*> GraphicCore::GetSkyboxTexture()
{
	std::vector<Texture*> res(6);

	Singleton<SharedGraphicsResources> SinglRes;

	res[0] = &SinglRes->GetTextureRef("skybox/right.jpg");
	res[1] = &SinglRes->GetTextureRef("skybox/left.jpg");
	res[2] = &SinglRes->GetTextureRef("skybox/top.jpg");
	res[3] = &SinglRes->GetTextureRef("skybox/bottom.jpg");
	res[4] = &SinglRes->GetTextureRef("skybox/front.jpg");
	res[5] = &SinglRes->GetTextureRef("skybox/back.jpg");
	return res;
}

GLShader& GraphicCore::GetSkyboxShader()
{
	Singleton<SharedGraphicsResources> SinglRes;
	return SinglRes->GetShaderRef("Shaders/Skybox.ueshad");
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

GraphicCore& GraphicCore::GetInstance()
{
	static GraphicCore  instance;
	return instance;
}

void GraphicCore::UpdateGraphic()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	processInput(WindowApp::GetInstance().GetWindow());



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

	SkyboxThat.Draw(view, projection);
	glfwSwapBuffers(WindowApp::GetInstance().GetWindow());
}