#include "ECS\Systems\Systems.h"
#include "ECS\Components\Game\RoomTraveler.h"

void Labyrinth::RoomTravelerController(ecs::EntityManager& em)
{
	for (auto l = em.GetComponents<RoomTraveler>(); !l.end(); ++l)
	{
		auto [roomTraveler] = *l;

		//for (auto l = em.GetComponents<Door>(); !l.end(); ++l)
		//{
			//auto [door] = *l;

		//}
	}
}
