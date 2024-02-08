#pragma once
#include <glm/glm.hpp>
#include "ECS\EcsSystem.h"
#include "GameTools\GraphGenerator.h"
#include "Utils\SimpleTextProcessor.h"

struct Room
{
	std::vector<glm::vec2> DoorPos;
	std::vector<int> DoorRoom;
};

struct LabyrinthData : public ecs::Component<LabyrinthData>
{
	static constexpr std::string_view ComponentName = "LabyrinthData";

	Graph levelGraph = Graph(34581, 8, 4, 4, 8, 4);
	std::vector<Room> rooms;

	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
	{
		auto& ld = em.GetComponent<LabyrinthData>(ent);
		
		auto counts = TextTools::ReadIntArray(res["Counts"]);
		ld.levelGraph = Graph(std::stoi(res["Seed"]), std::stoi(res["Length"]), counts[0], counts[1], counts[2], counts[3]);

		ld.rooms = std::vector<Room>(ld.levelGraph.Verts.size());

		static const std::vector<glm::vec2> pos = { {-3, 0}, {-1, 0}, {1, 0}, {3, 0} };

		for (int i = 0; i < ld.rooms.size(); ++i)
		{
			for (int j = 0; j < ld.levelGraph.Verts[i].size(); ++j)
			{
				ld.rooms[i].DoorPos.push_back(pos[j]);
				ld.rooms[i].DoorRoom.push_back(ld.levelGraph.Verts[i][j]);
			}
		}
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