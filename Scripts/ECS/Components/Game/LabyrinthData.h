#pragma once
#include <vector>
#include <array>
#include <glm/glm.hpp>
#include "ECS\EcsSystem.h"
#include "GameTools\GraphGenerator.h"
#include "Utils\SimpleTextProcessor.h"

enum class RoomSide
{
	LeftWall,
	CenterWall,
	RightWall,
	DownWall,
	TopWall
};

struct FlatCoord
{
	RoomSide side;
	glm::ivec2 pos;
};

struct DecorData
{
	FlatCoord pos;
	glm::vec<2, int> size;
};

struct DoorDataD
{
	FlatCoord pos;
	glm::vec<2, int> size;
	int nextRoom = 0;
};

struct Room
{
	std::vector<DoorDataD> doorData;
	std::vector<DecorData> decorData;
};

struct LabyrinthData : public ecs::Component<LabyrinthData>
{
	static constexpr std::string_view ComponentName = "LabyrinthData";

	Graph levelGraph = Graph(34581, 8, 4, 4, 8, 4);
	std::vector<Room> rooms;
	bool isGenerated = false;

	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
	{
		auto& ld = em.GetComponent<LabyrinthData>(ent);
		
		auto counts = TextTools::ReadIntArray(res["Counts"]);
		ld.levelGraph = Graph(std::stoi(res["Seed"]), std::stoi(res["Length"]), counts[0], counts[1], counts[2], counts[3]);
	}
};

struct DoorData : public ecs::Component<DoorData>
{
	static constexpr std::string_view ComponentName = "DoorData";

	int NextRoom;

	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
	{
		auto& dd = em.GetComponent<DoorData>(ent);
	}
};