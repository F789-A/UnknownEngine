#include "GameLoop.h"
#include "ecs_EntityManager.h"
#include "UIHandler.h"
#include "RectTransformComponent.h"
#include "PerspectiveBuilder.h"
#include "LineDrawSystem.h"
#include "AimData.h"
#include "AimSystem.h"

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

	//graphics
	GraphicCore::GetInstance().mainPass.push_back(DrawLine);

	//ecs sustems
	//ui
	ECS::DefEcs_().system.systemsPtr.push_back(ui::ProcessButtons);
	ECS::DefEcs_().system.systemsPtr.push_back(ui::DrawUIImage);
	//game
	ECS::DefEcs_().system.systemsPtr.push_back(AsteroidHunter::CharacterController);
	ECS::DefEcs_().system.systemsPtr.push_back(AsteroidHunter::AlienController);
	//int a = ECS::DefEcs_().entity.AddEntity<RectTransform, Image, Button>();

	//GraphicCore::GetInstance().mainPass.push_back(PerspectiveBuild);
}

float GameLoop::GetDeltaTime() const
{
	return DeltaTime;
}