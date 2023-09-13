#include "GameLoop.h"
#include "EcsSystem.h"

#include "PerspectiveBuilder.h"
#include "Camera.h"
#include "PostProcessComponent.h"
#include "RenderLineFiller.h"

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
		RenderMeshSystem(ecs::DefEcs().entity); 
		graphics::RenderSkyboxSystem(ecs::DefEcs().entity);
	};

	GraphicCore::GetInstance().uiPassFunc = []() {
		ui::DrawUIImage(ecs::DefEcs().entity);
		DrawLine(ecs::DefEcs().entity);
		ui::DrawText(ecs::DefEcs().entity);
	};

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
	ecs::DefEcs().system.AddSystem(AsteroidHunter::LevelController);
}

float GameLoop::GetDeltaTime() const
{
	return DeltaTime;
}