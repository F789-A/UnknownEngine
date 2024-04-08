#include "ECS\Systems\Systems.h"
#include <iostream>
#include "ECS\Components\Game\LabyrinthData.h"
#include "ECS\Components\Game\RoomRedrawer.h"

enum class RoomSide
{
	LeftWall,
	CenterWall,
	RightWall,
	DownWall,
	TopWall
};

glm::vec3 FlatCoordinateToSpace(const glm::vec<3, int>& coord)
{
	glm::vec3 roomSize{10.0f, 7.5f, 6.0f};
	glm::vec<3, int> gridSize{ 12, 9, 5 };

	auto side = static_cast<RoomSide>(coord.x);

	glm::vec3 ldCorner{ -roomSize.x / 2.0f, -roomSize.y / 2.0f, 0.0f };
	glm::vec3 divisions{ roomSize.x / gridSize.x, roomSize.y / gridSize.y, roomSize.z / gridSize.z };

	if (side == RoomSide::LeftWall)
	{
		return ldCorner + glm::vec3{ 0.0f, divisions.y * coord.z, divisions.z * coord.x };
	}
	else if (side == RoomSide::CenterWall)
	{
		return ldCorner + glm::vec3{ divisions.x * coord.y, divisions.y * coord.z, roomSize.z };
	}
	else if (side == RoomSide::RightWall)
	{
		return ldCorner + glm::vec3{ roomSize.x, divisions.y * coord.z, divisions.z * coord.x };
	}
	else if (side == RoomSide::DownWall)
	{
		return ldCorner + glm::vec3{ divisions.x * coord.y, 0.0f, divisions.z * coord.z };
	}
	else if (side == RoomSide::TopWall)
	{
		return ldCorner + glm::vec3{ divisions.x * coord.y, roomSize.z, divisions.z * coord.z };
	}
	throw;
}

glm::vec<3, int> GetRandomFromWall(RoomVisual& visual)
{
	std::uniform_int_distribution<> sideDistrib(0, 2);
	RoomSide side = static_cast<RoomSide>(sideDistrib(visual.random_generator));
	std::uniform_int_distribution<> cellDistribY(0, visual.BoxYCount);
	if (side == RoomSide::LeftWall || side == RoomSide::RightWall)
	{
		std::uniform_int_distribution<> cellDistrib(0, visual.BoxZCount);
		return { side, cellDistrib(visual.random_generator), cellDistribY(visual.random_generator) };
	}
	if (side == RoomSide::CenterWall)
	{
		std::uniform_int_distribution<> cellDistrib(0, visual.BoxXCount);
		return { side, cellDistrib(visual.random_generator), cellDistribY(visual.random_generator) };
	}
	throw;
}

glm::vec<3, int> GetRandomFromDownOfWall(RoomVisual& visual)
{
	auto result = GetRandomFromWall(visual);
	return { result.x, result.y, 0.0,};
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

				static const std::vector<glm::vec3> pos = {
					{-5.0f, -3.75f, 3.0f}, 
					{-3.0f, -3.75f, 6.0f},
					{3.0f, -3.75f, 6.0f},
					{5.0f, -3.75f, 3.0f} };
				ld.rooms[i].DoorPos.push_back(FlatCoordinateToSpace(cell));
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