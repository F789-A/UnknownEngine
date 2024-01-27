#include "ECS\Systems\Systems.h"

#include <random>
#include <glm\glm.hpp>

#include "ECS\Components\Game\LevelData.h"
#include "ECS\Components\Game\AlienData.h"
#include "ECS\Components\Events.h"

#include "SerializationSystem\EntityLoader.h"
#include "Utils\SafeSingleton.h"

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

void AddAlien(ecs::EntityManager& em, const glm::vec2& pos, AlienTypes type, float health, float damage, float support)
{
	SerializationSystem::LoadEntity(em, "Scenes\\AlienPrefab.txt");
	for (auto l = em.GetComponents<NewPlacedObject, RectTransform, AlienData>(); !l.end(); ++l)
	{
		auto [tag, transf, alienData] = *l;
		em.RemoveComponent<NewPlacedObject>(em.GetEntity(tag));
		transf.pos = pos;
		alienData.alienType = type;
		alienData.Health = health;
		alienData.Damage = damage;
		//alienData.Support = support;
	}
}

void AddAlientInRandomPlace(ecs::EntityManager& em, AlienTypes type, float health, float damage, float support)
{
	Box2d targetBox{ {-1, -1}, {1, 1} };
	glm::vec2 pos = RandomFromEdgeBox(targetBox);
	AddAlien(em, pos, type, health, damage, support);
}

void AsteroidHunter::LevelController(ecs::EntityManager& em)
{
	for (auto l = em.GetComponents<LevelData>(); !l.end(); ++l)
	{
		auto [levelData] = *l;

		if (levelData.countAlien < 2)
		{
			AddAlientInRandomPlace(em, AlienTypes::Coward | AlienTypes::PingPonger, 15, 1, 1);
			levelData.countAlien++;
		}
	}
	
}