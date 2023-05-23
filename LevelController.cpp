#include "Systems.h"
#include "TagController.h"
#include "SafeSingleton.h"
#include "LevelData.h"
#include "EntityLoader.h"

void AsteroidHunter::LevelController(ecs::EntityManager&)
{
	auto& levelData = InstanceOf<TagController>().GetComponentWithTag<LevelData>("LevelData");

	if (levelData.countAlien < 2)
	{
		SerializationSystem::LoadEntity(ecs::DefEcs().entity, "Scenes\\AlienPrefab.txt");
		levelData.countAlien++;
	}
}