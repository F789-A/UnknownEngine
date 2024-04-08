#pragma once
#include <vector>
#include <array>
#include <glm/glm.hpp>
#include "ECS\EcsSystem.h"
#include "GameTools\GraphGenerator.h"
#include "Utils\SimpleTextProcessor.h"

struct RoomObjectData
{
	glm::vec<3, int> pos;
	glm::vec<2, int> size;
	//texture
};

struct Room
{
	std::vector<glm::vec3> DoorPos;
	std::vector<int> DoorRoom;

	std::vector<RoomObjectData> roomData;
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