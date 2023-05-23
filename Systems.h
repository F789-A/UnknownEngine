#pragma once
#include "EcsSystem.h"

namespace ui
{
	void ProcessButtons(ecs::EntityManager&);
	void DrawUIImage(ecs::EntityManager&);
	void DrawText(ecs::EntityManager&);
}

namespace gr
{

}

void RenderMeshSystem(ecs::EntityManager&);

void EscapeHandler(ecs::EntityManager&);

void DrawLine(ecs::EntityManager&);

void CameraControllerSystem(ecs::EntityManager&);

namespace AsteroidHunter
{
	void CharacterController(ecs::EntityManager&);
	void AlienController(ecs::EntityManager&);
	void MenuEvent(ecs::EntityManager&);
	void LevelChanger(ecs::EntityManager&);
	void LevelController(ecs::EntityManager&);
}