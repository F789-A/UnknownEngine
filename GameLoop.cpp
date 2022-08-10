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
	EntityLoader h("Entites\\entityList.txt");
	h.LoadKeyFromFile("Key.txt");
	h.Load();
}

float GameLoop::GetDeltaTime()
{
	return DeltaTime;
}