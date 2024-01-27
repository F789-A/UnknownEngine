#include "GameLoop.h"

#include <vector>

#include "ECS\EcsSystem.h"

#include "ECS\Components\Camera.h"
#include "ECS\Components\PostProcessComponent.h"
#include "ECS\Components\Physics\DynamicBvh2D.h"

#include "ECS\Systems\Systems.h"

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

	//graphics
	GraphicCore::GetInstance().GetCameraMatrices = []() {
		for (auto l = ecs::DefEcs().entity.GetComponents<MainCamera, Camera>(); !l.end(); ++l)
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
		for (auto l = ecs::DefEcs().entity.GetComponents<PostProcessComponent>(); !l.end(); ++l)
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
		graphics::RenderMeshSystem(ecs::DefEcs().entity);
		graphics::RenderSkyboxSystem(ecs::DefEcs().entity);
	};

	GraphicCore::GetInstance().uiPassFunc = []() {
		ui::DrawUIImage(ecs::DefEcs().entity);
		ui::DrawLine(ecs::DefEcs().entity);
		ui::DrawText(ecs::DefEcs().entity);
	};

	//ecs sustems

	//ui
	ecs::DefEcs().system.AddSystem(ui::ProcessButtons);

	//physics
	ecs::DefEcs().system.AddSystem(physics::BuildBvh);
	ecs::DefEcs().system.AddSystem(physics::GravityController);
	ecs::DefEcs().system.AddSystem(physics::ProcessCollision);
	ecs::DefEcs().system.AddSystem(physics::ProcessMovement);
	
	//game
	ecs::DefEcs().system.AddSystem(GameTools::EscapeHandler);
	ecs::DefEcs().system.AddSystem(GameTools::CameraControllerSystem);
	ecs::DefEcs().system.AddSystem(GameTools::Camera2DControllerSystem);
	ecs::DefEcs().system.AddSystem(GameTools::ImpulseControllerSystem);
	ecs::DefEcs().system.AddSystem(GameTools::LevelChanger);
	ecs::DefEcs().system.AddSystem(AsteroidHunter::CharacterController);
	ecs::DefEcs().system.AddSystem(AsteroidHunter::AlienController);
	ecs::DefEcs().system.AddSystem(AsteroidHunter::MenuEvent);
	ecs::DefEcs().system.AddSystem(AsteroidHunter::LevelController);
	ecs::DefEcs().system.AddSystem(Labyrinth::GraphDebugger);
	ecs::DefEcs().system.AddSystem(Labyrinth::RoomTravelerController);
	ecs::DefEcs().system.AddSystem(Labyrinth::RoomRedrawerController);
}

float GameLoop::GetDeltaTime() const
{
	return DeltaTime;
}