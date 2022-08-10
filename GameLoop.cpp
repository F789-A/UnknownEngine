#include "GameLoop.h"


GameLoop& GameLoop::GetInstance()
{
	static GameLoop  instance;
	return instance;
}

void GameLoop::RunLoop()
{
	ConstructScene();
	Loop();
}

void GameLoop::Loop()
{
	static GLfloat lastFrame = 0.0f;

	while (!WindowApp::GetInstance().ShouldClose())
	{
		GLfloat currentFrame = glfwGetTime();
		DeltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		Input::GetInstance().UpdateInput();

		for (int i = 0; i < ILoopUpdate<UpdateType::GameLoop>::UpdateVector.size(); i++)
		{
			ILoopUpdate<UpdateType::GameLoop>::UpdateVector[i]->Update();
		}
		GraphicCore::GetInstance().UpdateGraphic();
	}
}

void GameLoop::ConstructScene()
{
	Input::GetInstance().BindKey("Front", GLFW_KEY_W);
	Input::GetInstance().BindKey("Right", GLFW_KEY_D);
	Input::GetInstance().BindKey("Left", GLFW_KEY_A);
	Input::GetInstance().BindKey("Backward", GLFW_KEY_S);
	EntityLoader h("Entites\\entityList.txt");
	h.Load();
}

float GameLoop::GetDeltaTime()
{
	return DeltaTime;
}