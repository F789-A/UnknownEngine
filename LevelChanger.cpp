#include "Systems.h"
#include "ChangeLevel.h"
#include "EntityLoader.h"
#include "Input.h"

void AsteroidHunter::LevelChanger(ecs::EntityManager& em)
{
	for (auto l = em.GetComponents<ChangeLevel>(); !l.end(); ++l)
	{
		auto [lvl] = *l;
		if (Input::GetInstance().GetButton("ChangeLevel", Input::PressMode::Press))
		{
			ecs::EntityManager tmpEm;
			SerializationSystem::LoadEntity(tmpEm, lvl.NextLevel);
			ecs::DefEcs_().SetEntityManager(std::move(tmpEm));
		}
	}
}