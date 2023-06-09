#include "Systems.h"
#include "TagController.h"
#include "SafeSingleton.h"
#include "LevelData.h"
#include "EntityLoader.h"

void AsteroidHunter::LevelController(ecs::EntityManager& em)
{
	for (auto l = em.GetComponents<LevelData>(); !l.end(); ++l)
	{
		//auto& levelData = em.GetComponent<LevelData>(InstanceOf<TagController>().GetEntityWithTag("LevelData"));
		auto [levelData] = *l;

		if (levelData.countAlien < 2)
		{
			SerializationSystem::LoadEntity(ecs::DefEcs().entity, "Scenes\\AlienPrefab.txt");
			levelData.countAlien++;
		}
	}
	
}