#include "ECS\Systems\Systems.h"
#include <iostream>
#include "ECS\Components\Game\Labyrinth\LabyrinthData.h"
#include "ECS\Components\Game\Labyrinth\RoomRedrawer.h"
#include "ECS\Components\Game\Labyrinth\SpriteStorage.h"

FlatCoord GetRandomFromSide(RoomVisual& visual, RoomSide side)
{
	std::uniform_int_distribution<> cellDistribX(0, visual.BoxXCount);
	std::uniform_int_distribution<> cellDistribY(0, visual.BoxYCount);
	std::uniform_int_distribution<> cellDistribZ(0, visual.BoxZCount);
	if (side == RoomSide::LeftWall || side == RoomSide::RightWall)
	{
		return { side, {cellDistribZ(visual.random_generator), cellDistribY(visual.random_generator)} };
	}
	if (side == RoomSide::CenterWall)
	{
		return { side, {cellDistribX(visual.random_generator), cellDistribY(visual.random_generator)} };
	}
	if (side == RoomSide::TopWall || side == RoomSide::DownWall)
	{
		return { side, {cellDistribX(visual.random_generator), cellDistribZ(visual.random_generator)} };
	}
	throw;
}

FlatCoord GetRandomFromWall(RoomVisual& visual)
{
	std::uniform_int_distribution<> sideDistrib(0, 2);
	RoomSide side = static_cast<RoomSide>(sideDistrib(visual.random_generator));
	return GetRandomFromSide(visual, side);
}

FlatCoord GetRandomFromDownOfWall(RoomVisual& visual)
{
	auto result = GetRandomFromWall(visual);
	return { result.side, {result.pos.x, 0.0} };
}

FlatCoord GetRandomFromAllWalls(RoomVisual& visual)
{
	std::uniform_int_distribution<> sideDistrib(0, 4);
	RoomSide side = static_cast<RoomSide>(sideDistrib(visual.random_generator));
	return GetRandomFromSide(visual, side);
}

bool CheckFreePlace(const FlatCoord& coord, const glm::ivec2& size, const std::array<std::vector<std::vector<bool>>, 5>& grid)
{
	auto wall = grid[static_cast<int>(coord.side)];

	if (wall.size() < coord.pos.x + size.x || wall[0].size() < coord.pos.y + size.y)
	{
		return false;
	}

	for (int n = 0; n < size.x; ++n)
	{
		for (int m = 0; m < size.y; ++m)
		{
			if (wall[coord.pos.x + n][coord.pos.y + m])
			{
				return false;
			}
		}
	}
	return true;
}

void Labyrinth::RoomsGenerator(ecs::EntityManager& em)
{
	for (auto l = em.GetComponents<LabyrinthData>(); !l.end(); ++l)
	{
		auto [ld] = *l;

		if (ld.isGenerated)
		{
			return;
		}
		ld.isGenerated = true;

		auto [visual] = *em.GetComponents<RoomVisual>();
		auto [tdoor, doorSpriteStorage] = *em.GetComponents<DoorSpriteStorageTag, SpriteStorage>();
		auto [tdec, decorSpriteStorage] = *em.GetComponents<DecorationsSpriteStorageTag, SpriteStorage>();

		ld.rooms = std::vector<Room>(ld.levelGraph.Verts.size());
		for (int i = 0; i < ld.rooms.size(); ++i) // gen room
		{
			std::array<std::vector<std::vector<bool>>, 5> gridHolder;
			gridHolder[static_cast<int>(RoomSide::LeftWall)] = std::vector<std::vector<bool>>(visual.BoxZCount, std::vector<bool>(visual.BoxYCount));
			gridHolder[static_cast<int>(RoomSide::CenterWall)] = std::vector<std::vector<bool>>(visual.BoxXCount, std::vector<bool>(visual.BoxYCount));
			gridHolder[static_cast<int>(RoomSide::RightWall)] = std::vector<std::vector<bool>>(visual.BoxZCount, std::vector<bool>(visual.BoxYCount));
			gridHolder[static_cast<int>(RoomSide::DownWall)] = std::vector<std::vector<bool>>(visual.BoxXCount, std::vector<bool>(visual.BoxZCount));
			gridHolder[static_cast<int>(RoomSide::TopWall)] = std::vector<std::vector<bool>>(visual.BoxZCount, std::vector<bool>(visual.BoxYCount));

			for (int j = 0; j < ld.levelGraph.Verts[i].size(); ++j) // gen doors
			{
				std::uniform_int_distribution<> spriteDistrib(0, doorSpriteStorage.Sizes.size()-1);
				int sprite = spriteDistrib(visual.random_generator);

				const glm::ivec2& size = doorSpriteStorage.Sizes[sprite];
				FlatCoord cell;
				bool placed = false;
				while (!placed) 
				{
					cell = GetRandomFromDownOfWall(visual);
					placed = CheckFreePlace(cell, size, gridHolder);
				}

				for (int n = 0; n < size.x; ++n)
				{
					for (int m = 0; m < size.y; ++m)
					{
						gridHolder[static_cast<int>(cell.side)][cell.pos.x + n][cell.pos.y + m] = true;
					}
				}

				ld.rooms[i].doorData.push_back({ cell, size, doorSpriteStorage.Materials[sprite], ld.levelGraph.Verts[i][j] });
			}

			if (i == 7)
			{
				ld.rooms[i].isWin = true;
			}

			for (int j = 0; j < visual.MaxAmountOfDecor; ++j) // gen decors
			{
				std::uniform_int_distribution<> spriteDistrib(0, decorSpriteStorage.Sizes.size() - 1);
				int sprite = spriteDistrib(visual.random_generator);

				const glm::ivec2& size = decorSpriteStorage.Sizes[sprite];
				FlatCoord cell;
				bool placed = false;
				while (!placed)
				{
					cell = GetRandomFromAllWalls(visual); // fix
					placed = CheckFreePlace(cell, size, gridHolder);
				}

				for (int n = 0; n < size.x; ++n)
				{
					for (int m = 0; m < size.y; ++m)
					{
						gridHolder[static_cast<int>(cell.side)][cell.pos.x + n][cell.pos.y + m] = true;
					}
				}

				ld.rooms[i].decorData.push_back({ cell, size, decorSpriteStorage.Materials[sprite] });
			}
		}

		
		
	}
}