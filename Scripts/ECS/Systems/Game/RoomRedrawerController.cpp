#include "ECS\Systems\Systems.h"
#include <glm/glm.hpp>
#include "ECS\Components\Game\RoomRedrawer.h"
#include "SerializationSystem\EntityLoader.h"
#include "ECS\Components\Events.h"
#include "ECS\Components\Game\LabyrinthData.h"
#include "ECS\Components\Game\RoomTraveler.h"
#include "ECS\Components\Transform.h"
#include "Ecs\Components\RenderMesh.h"
#include "Physics\Shapes.h"
#include "Core\Input.h"

using vec2int = glm::vec<2, int>;

glm::vec3 BoxToPyramid(glm::vec3 pos, glm::vec3 peak)
{
	return pos.z / peak.z * peak + (1.0f - pos.z / peak.z) * pos;
}

void AddObjectInRoom(int side, vec2int pos, vec2int size)
{
	std::vector<Vertex2D> points = {
		{ {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} },
		{ {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f} },
		{ {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f} },
		{ {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f} }
	};
	//GLMesh b(points, std::vector<GLuint>({ 0, 1, 2, 0, 2, 3}));
}

void Labyrinth::RoomRedrawerController(ecs::EntityManager& em)
{
	for (auto l = em.GetComponents<RoomRedrawer, LabyrinthData, RoomTraveler>(); !l.end(); ++l)
	{
		auto [redrawer, data, traveler] = *l;

		if (!redrawer.NeedRedraw)
		{
			return;
		}

		for (auto ent : redrawer.Entites)
		{
			em.RemoveEntity(ent);
		}
		redrawer.Entites.clear();

		for (int i = 0; i < data.levelGraph.Verts[traveler.CurrentRoom].size(); ++i)
		{
			SerializationSystem::LoadEntity(em, "Scenes\\DoorPrefab.txt");
		}

		int  i = 0;
		for (auto l = em.GetComponents<Transform, DoorData, NewPlacedObjectTag>(); !l.end(); ++l)
		{
			auto [tr, door, tag] = *l;

			glm::vec2 pos = data.rooms[traveler.CurrentRoom].DoorPos[i];

			tr.Position = glm::vec3(pos, 1.0f);

			door.NextRoom = data.levelGraph.Verts[traveler.CurrentRoom][i];
			i++;

			redrawer.Entites.push_back(em.GetEntity(tag));
			em.RemoveComponent<NewPlacedObjectTag>(em.GetEntity(tag));
		}

		redrawer.NeedRedraw = false;
	}
}

void Labyrinth::RoomBackgroundRedrawer(ecs::EntityManager& em)
{
	for (auto l = em.GetComponents<RoomTag, RenderMesh>(); !l.end(); ++l)
	{
		auto [tag, renderMesh] = *l;
		em.RemoveComponent<RoomTag>(em.GetEntity(tag));

		auto [visual] = *em.GetComponents<RoomVisual>();

		static constexpr float width = 10.0f;
		static constexpr float height = 7.5f;
		static constexpr glm::vec2 luCorner{ -width / 2.0f, height / 2.0f };
		static constexpr glm::vec2 ruCorner{ width / 2.0f, height / 2.0f };
		static constexpr glm::vec2 rdCorner{ width / 2.0f, -height / 2.0f };
		static constexpr glm::vec2 ldCorner{ -width / 2.0f, -height / 2.0f };

		float length = visual.CenterPos.z * visual.CenterScale;
		glm::vec2 lUp = BoxToPyramid(glm::vec3(luCorner, 0.0f) + glm::vec3(0.0f, 0.0f, length), visual.CenterPos);
		glm::vec2 rUp = BoxToPyramid(glm::vec3(ruCorner, 0.0f) + glm::vec3(0.0f, 0.0f, length), visual.CenterPos);
		glm::vec2 rDown = BoxToPyramid(glm::vec3(rdCorner, 0.0f) + glm::vec3(0.0f, 0.0f, length), visual.CenterPos);
		glm::vec2 lDown = BoxToPyramid(glm::vec3(ldCorner, 0.0f) + glm::vec3(0.0f, 0.0f, length), visual.CenterPos);

		std::vector<Vertex> linePoints =
		{
			{ {lUp, 0.0f} }, { {rUp, 0.0f} }, { {rDown, 0.0f} }, { {lDown, 0.0f} },
			{ {luCorner, 0.0f} }, { {ruCorner, 0.0f} }, { {rdCorner, 0.0f} }, { {ldCorner, 0.0f} }
		};

		GLMesh b(linePoints, std::vector<GLuint>({ 0, 1, 1, 2, 2, 3, 3, 0, 4, 0, 5, 1, 6, 2, 7, 3 }), GLMesh::GeometryTypes::Lines);
		renderMesh.RenderedMesh = std::move(b);
	}
}

void Labyrinth::RoomGridDrawer(ecs::EntityManager& em)
{
	for (auto l = em.GetComponents<GridController, RenderMesh, Transform>(); !l.end(); ++l)
	{
		auto [grid, renderMesh, tr] = *l;
		
		auto [visual] = *em.GetComponents<RoomVisual>();

		if (!grid.isDrawed)
		{
			grid.isDrawed = true;

			static constexpr float width = 10.0f;
			static constexpr float height = 7.5f;
			static constexpr glm::vec2 luCorner{ -width / 2.0f, height / 2.0f };
			static constexpr glm::vec2 ruCorner{ width / 2.0f, height / 2.0f };
			static constexpr glm::vec2 rdCorner{ width / 2.0f, -height / 2.0f };
			static constexpr glm::vec2 ldCorner{ -width / 2.0f, -height / 2.0f };

			std::vector<Vertex> linePoints;
			std::vector<GLuint> indices;
			
			float length = visual.CenterPos.z * visual.CenterScale;
			float xLen = width / visual.BoxXCount;
			float yLen = height / visual.BoxYCount;
			float zLen = length / visual.BoxZCount;

			for (int i = 1; i < visual.BoxXCount; ++i)
			{
				float xCoord = luCorner.x + (float)i * xLen;
				linePoints.push_back({ {xCoord, luCorner.y, 0.0f} });
				linePoints.push_back({ {glm::vec2(BoxToPyramid({ xCoord, luCorner.y, length}, visual.CenterPos)), 0.0f} });
				linePoints.push_back({ {glm::vec2(BoxToPyramid({ xCoord, ldCorner.y, length}, visual.CenterPos)), 0.0f} });
				linePoints.push_back({ {xCoord, ldCorner.y, 0.0f} });
			}
			for (int i = 1; i < visual.BoxYCount; ++i)
			{
				float yCoord = ldCorner.y + (float)i * yLen;
				linePoints.push_back({ {ldCorner.x, yCoord, 0.0f} });
				linePoints.push_back({ {glm::vec2(BoxToPyramid({ ldCorner.x, yCoord, length}, visual.CenterPos)), 0.0f} });
				linePoints.push_back({ {glm::vec2(BoxToPyramid({ rdCorner.x, yCoord, length}, visual.CenterPos)), 0.0f} });
				linePoints.push_back({ {rdCorner.x, yCoord, 0.0f} });
			}

			int prevCount = linePoints.size();
			for (int i = 0; i < linePoints.size(); i+= 4)
			{
				indices.push_back(i);
				indices.push_back(i+1);
				indices.push_back(i+1);
				indices.push_back(i+2);
				indices.push_back(i+2);
				indices.push_back(i+3);
			}

			for (int i = 1; i < visual.BoxZCount; ++i)
			{
				float zCoord = zLen * (float)i;
				linePoints.push_back({ {glm::vec2(BoxToPyramid({ luCorner.x, luCorner.y, zCoord}, visual.CenterPos)), 0.0f} });
				linePoints.push_back({ {glm::vec2(BoxToPyramid({ ruCorner.x, ruCorner.y, zCoord}, visual.CenterPos)), 0.0f} });
				linePoints.push_back({ {glm::vec2(BoxToPyramid({ rdCorner.x, rdCorner.y, zCoord}, visual.CenterPos)), 0.0f} });
				linePoints.push_back({ {glm::vec2(BoxToPyramid({ ldCorner.x, ldCorner.y, zCoord}, visual.CenterPos)), 0.0f} });
			}

			for (int i = prevCount; i < linePoints.size(); i += 4)
			{
				indices.push_back(i);
				indices.push_back(i + 1);
				indices.push_back(i + 1);
				indices.push_back(i + 2);
				indices.push_back(i + 2);
				indices.push_back(i + 3);
				indices.push_back(i + 3);
				indices.push_back(i);
			}

			GLMesh b(linePoints, indices, GLMesh::GeometryTypes::Lines);
			renderMesh.RenderedMesh = std::move(b);
		}

		if (Input::GetInstance().GetButton("UseE", Input::PressMode::Press))
		{
			grid.isActive = !grid.isActive;
			if (grid.isActive)
			{
				tr.Position = glm::vec3(0.0f, 0.0f, 0.0f);
			}
			else
			{
				tr.Position = glm::vec3(10.0f, 0.0f, 0.0f);
			}
		}
	}

}