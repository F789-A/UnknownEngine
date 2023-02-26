#include "EscapeHandler.h"

#include "Input.h"
#include "GLShader.h"
#include "SharedGraphicsResources.h"
#include "GraphicCore.h"

void EscapeHandler(ecs::EntityManager& em)
{
	Singleton<SharedGraphicsResources> SinglRes;
	GLShader& shad = SinglRes->GetShaderRef("Shaders/Pixelization.ueshad");
	static int a = 10000;
	if (Input::GetInstance().GetButton("Escape", Input::PressMode::Press))
	{
		glfwSetWindowShouldClose(WindowApp::GetInstance().GetWindow(), true);
	}
	if (Input::GetInstance().GetButton("UseQ", Input::PressMode::Press))
	{
		a--;
	}
	if (Input::GetInstance().GetButton("UseR", Input::PressMode::Press))
	{
		a++;
	}
	if (Input::GetInstance().GetButton("UseE", Input::PressMode::Press))
	{
		GraphicCore::GetInstance().EnablePostProcessing = !GraphicCore::GetInstance().EnablePostProcessing;
	}
	if (Input::GetInstance().GetButton("UseN", Input::PressMode::Press))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (Input::GetInstance().GetButton("UseM", Input::PressMode::Press))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (a < 1)
	{
		a = 1;
	}
	shad.SetInt("intencity", a / 100);
}