#include "GameLoop.h"

#include <vector>

#include "ECS\EcsSystem.h"

#include "ECS\Components\Camera.h"
#include "ECS\Components\PostProcessComponent.h"
#include "ECS\Components\Physics\DynamicBvh2D.h"

#include "ECS\Systems\Systems.h"

#include "WindowApp.h"
#include "Input.h"

#include "Graphics\GraphicCore.h"

#include "SerializationSystem\EntityLoader.h"

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
	static float lastFrame = 0.0f;
	while (!WindowApp::GetInstance().ShouldClose())
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		DeltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		Input::GetInstance().UpdateInput();
		ecs::EcsSystem::GetInstance().system.Update();

		GraphicCore::GetInstance().UpdateGraphic();
	}
}

void GameLoop::ConstructScene()
{
	SerializationSystem::LoadKeyFromFile(Input::GetInstance(), "Scenes\\Key.txt");
	SerializationSystem::LoadEntity(ecs::EcsSystem::GetInstance().entity, "Scenes\\DemoLevel.txt");

	//graphics
	GraphicCore::GetInstance().GetCameraMatrices = []() {
		for (auto l = ecs::EcsSystem::GetInstance().entity.GetComponents<MainCamera, Camera>(); !l.end(); ++l)
		{
			auto [tag, camera] = *l;
			glm::mat4 projection = camera.GetProjectionMatrix();
			glm::mat4 view = camera.GetViewMatrix();
			return std::make_pair(projection, view);
		}
		throw " ";
	};

	GraphicCore::GetInstance().GetPostProcesses = []() {
		std::vector<GLMaterial*> res;
		for (auto l = ecs::EcsSystem::GetInstance().entity.GetComponents<PostProcessComponent>(); !l.end(); ++l)
		{
			auto [postPr] = *l;
			if (postPr.IsEnabled)
			{
				res.push_back(&postPr.RenderedMaterial);
			}
		}

		return res;
	};

	GraphicCore::GetInstance().mainPassFunc = []() {
		graphics::RenderMeshSystem(ecs::EcsSystem::GetInstance().entity);
		graphics::RenderSkyboxSystem(ecs::EcsSystem::GetInstance().entity);
	};

	GraphicCore::GetInstance().uiPassFunc = []() {
		ui::DrawUIImage(ecs::EcsSystem::GetInstance().entity);
		ui::DrawLine(ecs::EcsSystem::GetInstance().entity);
		ui::DrawText(ecs::EcsSystem::GetInstance().entity);
	};

	//ecs sustems

	//ui
	ecs::EcsSystem::GetInstance().system.AddSystem(ui::ProcessButtons);

	//physics
	ecs::EcsSystem::GetInstance().system.AddSystem(physics::BuildBvh);
	ecs::EcsSystem::GetInstance().system.AddSystem(physics::GravityController);
	ecs::EcsSystem::GetInstance().system.AddSystem(physics::ProcessCollision);
	ecs::EcsSystem::GetInstance().system.AddSystem(physics::ProcessMovement);
	
	//game
	ecs::EcsSystem::GetInstance().system.AddSystem(GameTools::EscapeHandler);
	ecs::EcsSystem::GetInstance().system.AddSystem(GameTools::CameraControllerSystem);
	ecs::EcsSystem::GetInstance().system.AddSystem(GameTools::Camera2DControllerSystem);
	ecs::EcsSystem::GetInstance().system.AddSystem(GameTools::ImpulseControllerSystem);
	ecs::EcsSystem::GetInstance().system.AddSystem(GameTools::LevelChanger);
	ecs::EcsSystem::GetInstance().system.AddSystem(AsteroidHunter::CharacterController);
	ecs::EcsSystem::GetInstance().system.AddSystem(AsteroidHunter::AlienController);
	ecs::EcsSystem::GetInstance().system.AddSystem(AsteroidHunter::MenuEvent);
	ecs::EcsSystem::GetInstance().system.AddSystem(AsteroidHunter::LevelController);
	ecs::EcsSystem::GetInstance().system.AddSystem(Labyrinth::GraphDebugger);
	ecs::EcsSystem::GetInstance().system.AddSystem(Labyrinth::RoomTravelerController);
	ecs::EcsSystem::GetInstance().system.AddSystem(Labyrinth::RoomsGenerator);
	ecs::EcsSystem::GetInstance().system.AddSystem(Labyrinth::RoomRedrawerController);
	ecs::EcsSystem::GetInstance().system.AddSystem(Labyrinth::RoomBackgroundRedrawer);
	ecs::EcsSystem::GetInstance().system.AddSystem(Labyrinth::RoomGridDrawer);
	ecs::EcsSystem::GetInstance().system.AddSystem(Labyrinth::MainMenuController);
}

float GameLoop::GetDeltaTime() const
{
	return DeltaTime;
}