#include "ECS\Systems\Systems.h"
#include "ECS\Components\Game\LabyrinthData.h"
#include "ECS\Components\Game\RoomRedrawer.h"

glm::vec<3, int> GetRandomFromWall(RoomVisual& visual)
{
	std::uniform_int_distribution<> sideDistrib(0, 2);
	int side = sideDistrib(visual.random_generator);
	std::uniform_int_distribution<> cellDistribY(0, visual.BoxYCount);
	if (side == 0 || side == 2)
	{
		std::uniform_int_distribution<> cellDistrib(0, visual.BoxZCount);
		return { side, cellDistrib(visual.random_generator), cellDistribY(visual.random_generator) };
	}
	if (side == 1)
	{
		std::uniform_int_distribution<> cellDistrib(0, visual.BoxXCount);
		return { side, cellDistrib(visual.random_generator), cellDistribY(visual.random_generator) };
	}
	return {};
}

glm::vec<3, int> GetRandomFromDownOfWall(RoomVisual& visual)
{
	auto result = GetRandomFromWall(visual);
	return { result.x, result.y, 0.0 };
}

void Labyrinth::RoomsGenerator(ecs::EntityManager& em)
{
	for (auto l = em.GetComponents<LabyrinthData>(); !l.end(); ++l)
	{
		auto [ld] = *l;
		auto [visual] = *em.GetComponents<RoomVisual>();

		if (ld.isGenerated)
		{
			return;
		}
		ld.isGenerated = true;

		ld.rooms = std::vector<Room>(ld.levelGraph.Verts.size());

		for (int i = 0; i < ld.rooms.size(); ++i)
		{
			std::array<std::vector<std::vector<bool>>, 5> gridHolder;

			for (int j = 0; j < ld.levelGraph.Verts[i].size(); ++j)
			{
				glm::vec<3, int> cell = GetRandomFromDownOfWall(visual);

				ld.rooms[i].roomData.push_back({ cell, {4, 4} });

				static const std::vector<glm::vec2> pos = { {-3, 0}, {-1, 0}, {1, 0}, {3, 0} };
				ld.rooms[i].DoorPos.push_back(pos[j]);
				ld.rooms[i].DoorRoom.push_back(ld.levelGraph.Verts[i][j]);
			}

			for (int j = 0; j < 4; ++j)
			{
				glm::vec<3, int> cell = GetRandomFromWall(visual);
				ld.rooms[i].roomData.push_back({ cell, {4, 4} });
			}
		}

		
		
	}
}