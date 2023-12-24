#include "Systems.h"
#include "SafeSingleton.h"
#include "LevelData.h"
#include "EntityLoader.h"
#include "AlienData.h"
#include <glm\vec2.hpp>
#include <random>

struct Box2d
{
	glm::vec2 min;
	glm::vec2 max;
};

glm::vec2 RandomFromEdgeBox(Box2d box)
{
	glm::vec2 corner[4] = { box.min, {box.max.x, box.min.y}, box.max, {box.min.x, box.max.y} };

	static std::mt19937 rnd(0);
	std::uniform_int_distribution<int> distInt(0, 3); // include 4?	
	std::uniform_real_distribution<float> distFl(0, 1);

	int num = distInt(rnd);
	float mult = distFl(rnd);

	return corner[num] * mult + (1 - mult) * corner[(num+1)%4];
}

void AddAlien(const glm::vec2& pos, AlienTypes type, float health, float damage, float support)
{
	SerializationSystem::LoadEntity(ecs::DefEcs().entity, "Scenes\\AlienPrefab.txt");
	for (auto l = ecs::DefEcs().entity.GetComponents<NewPlacedAlienTag, RectTransform, AlienData>(); !l.end(); ++l)
	{
		auto [tag, transf, alienData] = *l;
		ecs::DefEcs().entity.RemoveComponent<NewPlacedAlienTag>(ecs::DefEcs().entity.GetEntity(tag));
		transf.pos = pos;
		alienData.alienType = type;
		alienData.Health = health;
		alienData.Damage = damage;
		//alienData.Support = support;
	}
}

void AddAlientInRandomPlace(AlienTypes type, float health, float damage, float support)
{
	Box2d targetBox{ {-1, -1}, {1, 1} };
	glm::vec2 pos = RandomFromEdgeBox(targetBox);
	AddAlien(pos, type, health, damage, support);
}

void AsteroidHunter::LevelController(ecs::EntityManager& em)
{
	for (auto l = em.GetComponents<LevelData>(); !l.end(); ++l)
	{
		auto [levelData] = *l;

		if (levelData.countAlien < 2)
		{
			AddAlientInRandomPlace(AlienTypes::Coward | AlienTypes::PingPonger, 15, 1, 1);
			//SerializationSystem::LoadEntity(ecs::DefEcs().entity, "Scenes\\AlienPrefab.txt");
			levelData.countAlien++;
		}
	}
	
}