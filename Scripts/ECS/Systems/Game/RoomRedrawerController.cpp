#include "ECS\Systems\Systems.h"
#include "ECS\Components\Game\RoomRedrawer.h"
#include "SerializationSystem\EntityLoader.h"
#include "ECS\Components\Events.h"
#include "ECS\Components\Game\LabyrinthData.h"
#include "ECS\Components\Game\RoomTraveler.h"
#include "ECS\Components\Game\GraphDebugInfo.h"
#include "ECS\Components\Transform.h"

void Labyrinth::RoomRedrawerController(ecs::EntityManager& em)
{
	for (auto l = em.GetComponents<RoomRedrawer, LabyrinthData, RoomTraveler>(); !l.end(); ++l)
	{
		auto [redrawer, data, traveler] = *l;

		if (!redrawer.NeedRedraw)
		{
			return;
		}

		for (auto ent : redrawer.Entites)
		{
			em.RemoveEntity(ent);
		}
		redrawer.Entites.clear();

		SerializationSystem::LoadEntity(em, "Scenes\\RoomPrefab.txt");
		for (int i = 0; i < data.levelGraph.Verts[traveler.CurrentRoom].size(); ++i)
		{
			SerializationSystem::LoadEntity(em, "Scenes\\DoorPrefab.txt");
		}

		int  i = 0;
		for (auto l = em.GetComponents<Transform, DoorData, NewPlacedObject>(); !l.end(); ++l)
		{
			auto [tr, door, tag] = *l;

			glm::vec2 pos = data.rooms[traveler.CurrentRoom].DoorPos[i];

			tr.Position = glm::vec3(pos, 1.0f);

			door.NextRoom = data.levelGraph.Verts[traveler.CurrentRoom][i];
			i++;
		}

		for (auto l = em.GetComponents<NewPlacedObject>(); !l.end(); ++l)
		{
			auto [tag] = *l;

			redrawer.Entites.push_back(em.GetEntity(tag));
			em.RemoveComponent<NewPlacedObject>(em.GetEntity(tag));
		}

		redrawer.NeedRedraw = false;
	}
}