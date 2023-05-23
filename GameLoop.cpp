#include "GameLoop.h"
#include "EcsSystem.h"

#include "PerspectiveBuilder.h"

#include "Systems.h"

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
		ecs::DefEcs().system.Update();

		GraphicCore::GetInstance().UpdateGraphic();
	}
}

void GameLoop::ConstructScene()
{
	SerializationSystem::LoadKeyFromFile(Input::GetInstance(),  "Scenes\\Key.txt");
	SerializationSystem::LoadEntity(ecs::DefEcs().entity, "Scenes\\DemoLevel.txt");

	Singleton<SharedGraphicsResources> singlRes;

	//graphics
	GraphicCore::GetInstance().mainPass.push_back(RenderMeshSystem);
	GraphicCore::GetInstance().UiPass.push_back(ui::DrawUIImage);
	GraphicCore::GetInstance().UiPass.push_back(DrawLine);
	GraphicCore::GetInstance().UiPass.push_back(ui::DrawText);

	//ecs sustems

	//ui
	ecs::DefEcs().system.AddSystem(ui::ProcessButtons);
	
	//game
	ecs::DefEcs().system.AddSystem(EscapeHandler);
	ecs::DefEcs().system.AddSystem(CameraControllerSystem);
	ecs::DefEcs().system.AddSystem(AsteroidHunter::CharacterController);
	ecs::DefEcs().system.AddSystem(AsteroidHunter::AlienController);
	ecs::DefEcs().system.AddSystem(AsteroidHunter::MenuEvent);
	ecs::DefEcs().system.AddSystem(AsteroidHunter::LevelChanger);
	//ecs::DefEcs().system.AddSystem(AsteroidHunter::LevelController);
}

float GameLoop::GetDeltaTime() const
{
	return DeltaTime;
}