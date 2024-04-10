#include "ECS\Systems\Systems.h"
#include <iostream>
#include "ECS\Components\Game\LabyrinthData.h"
#include "ECS\Components\Game\RoomRedrawer.h"

FlatCoord GetRandomFromWall(RoomVisual& visual)
{
	std::uniform_int_distribution<> sideDistrib(0, 2);
	RoomSide side = static_cast<RoomSide>(sideDistrib(visual.random_generator));
	std::uniform_int_distribution<> cellDistribY(0, visual.BoxYCount);
	if (side == RoomSide::LeftWall || side == RoomSide::RightWall)
	{
		std::uniform_int_distribution<> cellDistrib(0, visual.BoxZCount);
		return { side, {cellDistrib(visual.random_generator), cellDistribY(visual.random_generator)} };
	}
	if (side == RoomSide::CenterWall)
	{
		std::uniform_int_distribution<> cellDistrib(0, visual.BoxXCount);
		return { side, {cellDistrib(visual.random_generator), cellDistribY(visual.random_generator)} };
	}
	throw;
}

FlatCoord GetRandomFromDownOfWall(RoomVisual& visual)
{
	auto result = GetRandomFromWall(visual);
	return { result.side, {result.pos.x, 0.0} };
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
				FlatCoord cell = GetRandomFromDownOfWall(visual);

				ld.rooms[i].doorData.push_back({ cell, {2, 2}, ld.levelGraph.Verts[i][j] });
			}

			for (int j = 0; j < 4; ++j)
			{
				FlatCoord cell = GetRandomFromWall(visual);
				ld.rooms[i].decorData.push_back({ cell, {4, 4} });
			}
		}

		
		
	}
}