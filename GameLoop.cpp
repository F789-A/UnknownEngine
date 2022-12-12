#include "GameLoop.h"
#include "ecs_EntityManager.h"
#include "TestSystem.h"
#include "UIHandler.h"
#include "RectTransformComponent.h"
#include "PerspectiveBuilder.h"

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

	ECS::DefEcs_().system.Register<ButtonProcessor>(0);
	int a = ECS::DefEcs_().entity.AddEntity<RectTransform, Image, Button>();
	//ECS::DefEcs.entity.AddComponent<ecsComponent>(a);
	GraphicCore::GetInstance().funcs.push_back(PerspectiveBuild);
	while (!WindowApp::GetInstance().ShouldClose())
	{
		GLfloat currentFrame = glfwGetTime();
		DeltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		Input::GetInstance().UpdateInput();
		ECS::DefEcs_().system.Update();
		for (int i = 0; i < ILoopUpdate<UpdateType::GameLoop>::UpdateVector.size(); i++)
		{
			ILoopUpdate<UpdateType::GameLoop>::UpdateVector[i]->Update();
		}

		GraphicCore::GetInstance().UpdateGraphic();
	}
}

void GameLoop::ConstructScene()
{
	SerializationSystem::LoadKeyFromFile("Key.txt");
	SerializationSystem::LoadEntity("Entites\\entityList.txt");
}

float GameLoop::GetDeltaTime() const
{
	return DeltaTime;
}