#include "GameLoop.h"
#include "ecs_EntityManager.h"

#include "UiComponents.h"
#include "PerspectiveBuilder.h"
#include "AimData.h"
#include "AimSystem.h"
#include "EscapeHandler.h"
#include "Transform.h"
#include "RenderMesh.h"
#include "CameraController.h"
#include "Camera.h"
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
		ECS::DefEcs_().system.Update();

		GraphicCore::GetInstance().UpdateGraphic();
	}
}

void GameLoop::ConstructScene()
{
	SerializationSystem::LoadKeyFromFile("Key.txt");
	SerializationSystem::LoadEntity("Entites\\entityList.txt");

	Singleton<SharedGraphicsResources> singlRes;

	//graphics
	GraphicCore::GetInstance().mainPass.push_back(DrawLine);
	GraphicCore::GetInstance().mainPass.push_back(RenderMeshSystem);
	GraphicCore::GetInstance().UiPass.push_back(ui::DrawUIImage);

	//ecs sustems

	//ui
	ECS::DefEcs_().system.systemsPtr.push_back(ui::ProcessButtons);
	
	//game
	ECS::DefEcs_().system.systemsPtr.push_back(EscapeHandler);
	ECS::DefEcs_().system.systemsPtr.push_back(CameraControllerSystem);
	ECS::DefEcs_().system.systemsPtr.push_back(AsteroidHunter::CharacterController);
	ECS::DefEcs_().system.systemsPtr.push_back(AsteroidHunter::AlienController);

	//int a = ECS::DefEcs_().entity.AddEntity<Transform, RenderMesh>();
	/*Transform& tr = ECS::DefEcs_().entity.GetComponent<Transform>(a);
	tr.Position = glm::vec3(0, 0, 20);
	RenderMesh& renMesh = ECS::DefEcs_().entity.GetComponent<RenderMesh>(a);
	renMesh.RenderedMesh = GLMesh(singlRes->ModelCont.GetModelRef("Models\\box.obj").Meshes[0]);
	renMesh.RenderMaterial = GLMaterial(singlRes->GetMaterial("Materials\\Diffuse_1.uemat"));*/

	int b = ECS::DefEcs_().entity.AddEntity<Transform, CameraController, Camera, MainCamera>();

	Transform& tr11 = ECS::DefEcs_().entity.GetComponent<Transform>(b);
	//GraphicCore::GetInstance().mainPass.push_back(PerspectiveBuild);
}

float GameLoop::GetDeltaTime() const
{
	return DeltaTime;
}