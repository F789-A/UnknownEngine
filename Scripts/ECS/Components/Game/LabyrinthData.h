#pragma once
#include <glm/glm.hpp>
#include "ECS\EcsSystem.h"
#include "GameTools\GraphGenerator.h"


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

	static void Load(ecs::EntityManager& em, int a, std::map<std::string, std::string>& res)
	{
		auto& ld = em.GetComponent<LabyrinthData>(a);
		ld.levelGraph = Graph(34581, 8, 4, 4, 8, 4);

		ld.rooms = std::vector<Room>(ld.levelGraph.Verts.size());

		static const std::vector<glm::vec2> pos = { {0, 0}, {1, 0}, {2, 0}, {3, 0} };

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