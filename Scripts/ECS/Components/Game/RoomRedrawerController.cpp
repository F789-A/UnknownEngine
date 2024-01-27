#include "ECS\Systems\Systems.h"
#include "RoomRedrawer.h"
#include "SerializationSystem\EntityLoader.h"
#include "ECS\Components\Events.h"
#include "LabyrinthData.h"
#include "RoomTraveler.h"
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

		redrawer.NeedRedraw = false;

		for (auto ent : redrawer.Entites)
		{
			em.RemoveEntity(ent);
		}
		redrawer.Entites.clear();

		SerializationSystem::LoadEntity(em, "Scenes\\RoomPrefab.txt");

		for (int i = 0; i < data.levelGraph.Verts[traveler.CurrentRoom].size(); ++i)
		{
			SerializationSystem::LoadEntity(em, "Scenes\\DoorPrefab.txt");

			for (auto l = em.GetComponents<Transform, NewPlacedObject>(); !l.end(); ++l)
			{
				auto [tr, tag] = *l;
				em.RemoveComponent<NewPlacedObject>(em.GetEntity(tag));

				glm::vec2 pos = data.rooms[traveler.CurrentRoom].DoorPos[i];

				tr.Position = glm::vec3(pos, 0.0f);
			}
		}

		redrawer.NeedRedraw = false;
	}
}