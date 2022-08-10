#pragma once
#include "ILoopUpdate.h"
#include "Component.h"
#include "Input\Input.h"


class EscapeHandler : public ec::Component, public ILoopUpdate<UpdateType::GameLoop>
{
	void Update()
	{
		if (Input::GetInstance().GetButton("Escape", GLFW_PRESS))
		{
			glfwSetWindowShouldClose(WindowApp::GetInstance().GetWindow(), true);
		}
		if (Input::GetInstance().GetButton("UseQ", GLFW_PRESS))
		{
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		if (Input::GetInstance().GetButton("UseR", GLFW_PRESS))
		{
			//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
	
};