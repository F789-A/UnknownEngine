#pragma once
#include "EcsSystem.h"

namespace ui
{
	void ProcessButtons(ecs::EntityManager&);
	void DrawUIImage(ecs::EntityManager&);
	void DrawText(ecs::EntityManager&);

	void DrawLine(ecs::EntityManager&);
}

namespace graphics
{
	void RenderMeshSystem(ecs::EntityManager&);
	void RenderSkyboxSystem(ecs::EntityManager&);
}

namespace physics
{
	void BuildBvh(ecs::EntityManager&);
	void GravityController(ecs::EntityManager&);
	void ProcessMovement(ecs::EntityManager&);
	void ProcessCollision(ecs::EntityManager&);
}

namespace GameTools
{
	void EscapeHandler(ecs::EntityManager&);
	void CameraControllerSystem(ecs::EntityManager&);
	void Camera2DControllerSystem(ecs::EntityManager&);
	void ImpulseControllerSystem(ecs::EntityManager&);
	void LevelChanger(ecs::EntityManager&);
}

namespace AsteroidHunter
{
	void CharacterController(ecs::EntityManager&);
	void AlienController(ecs::EntityManager&);
	void MenuEvent(ecs::EntityManager&);
	void LevelController(ecs::EntityManager&);
}

namespace Labyrinth
{
	void GraphDebugger(ecs::EntityManager&);
	void RoomTravelerController(ecs::EntityManager&);
	void RoomRedrawerController(ecs::EntityManager&);
}