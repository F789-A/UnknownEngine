#include "EscapeHandler.h"

#include "Input.h"
#include "GLShader.h"
#include "SharedGraphicsResources.h"
#include "GraphicCore.h"

void EscapeHandler()
{
	Singleton<SharedGraphicsResources> SinglRes;
	GLShader& shad = SinglRes->GetShaderRef("Shaders/Pixelization.ueshad");
	static int a = 10000;
	if (Input::GetInstance().GetButton("Escape", GLFW_PRESS))
	{
		glfwSetWindowShouldClose(WindowApp::GetInstance().GetWindow(), true);
	}
	if (Input::GetInstance().GetButton("UseQ", GLFW_PRESS))
	{
		a--;
	}
	if (Input::GetInstance().GetButton("UseR", GLFW_PRESS))
	{
		a++;
	}
	if (Input::GetInstance().GetButton("UseE", GLFW_PRESS))
	{
		GraphicCore::GetInstance().EnablePostProcessing = !GraphicCore::GetInstance().EnablePostProcessing;
	}
	if (Input::GetInstance().GetButton("UseN", GLFW_PRESS))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (Input::GetInstance().GetButton("UseM", GLFW_PRESS))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (a < 1)
	{
		a = 1;
	}
	shad.SetInt("intencity", a / 100);
}