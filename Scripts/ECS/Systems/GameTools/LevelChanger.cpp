#include "ECS\Systems\Systems.h"
#include "ECS\Components\GameTools\ChangeLevel.h"
#include "SerializationSystem\EntityLoader.h"
#include "Core\Input.h"

void GameTools::LevelChanger(ecs::EntityManager& em)
{
	for (auto l = em.GetComponents<ChangeLevel>(); !l.end(); ++l)
	{
		auto [lvl] = *l;
		if (Input::GetInstance().GetButton("ChangeLevel", Input::PressMode::Press))
		{
			ecs::EntityManager tmpEm;
			SerializationSystem::LoadEntity(tmpEm, lvl.NextLevel);
			ecs::EcsSystem::GetInstance().SetEntityManager(std::move(tmpEm));
		}
	}
}