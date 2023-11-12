#include "Systems.h"

#include "Input.h"
#include "GLShader.h"
#include "SharedGraphicsResources.h"
#include "GraphicCore.h"
#include "PostProcessComponent.h"
#include "WindowApp.h"

void GameTools::EscapeHandler(ecs::EntityManager& em)
{
	if (Input::GetInstance().GetButton("Escape", Input::PressMode::Press))
	{
		glfwSetWindowShouldClose(WindowApp::GetInstance().GetWindow(), true);
	}
	for (auto l = em.GetComponents<PostProcessComponent>(); !l.end(); ++l)
	{
		auto [postPr] = *l;
		if (Input::GetInstance().GetButton("UseE", Input::PressMode::Press))
		{
			postPr.IsEnabled = !postPr.IsEnabled;
		}
		int intencity = postPr.RenderedMaterial.ParametersInt["intencity"];
		if (Input::GetInstance().GetButton("UseQ", Input::PressMode::Repeat))
		{
			intencity--;
		}
		if (Input::GetInstance().GetButton("UseR", Input::PressMode::Repeat))
		{
			intencity++;
		}
		intencity = std::clamp(intencity, 1, 1000);
		postPr.RenderedMaterial.ParametersInt["intencity"] = intencity;
	}
	if (Input::GetInstance().GetButton("UseN", Input::PressMode::Press))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (Input::GetInstance().GetButton("UseM", Input::PressMode::Press))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}