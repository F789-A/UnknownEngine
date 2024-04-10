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

glm::vec3 BoxToPyramid(const glm::vec3& pos, const glm::vec3& peak)
{
	return pos.z / peak.z * peak + (1.0f - pos.z / peak.z) * pos;
}

glm::vec3 FlatCoordinateToSpace(const FlatCoord& coord)
{
	glm::vec3 roomSize{ 10.0f, 7.5f, 6.0f };
	glm::ivec3 gridSize{ 12, 9, 5 };

	glm::vec3 ldCorner{ -roomSize.x / 2.0f, -roomSize.y / 2.0f, 0.0f };
	glm::vec3 divisions{ roomSize.x / gridSize.x, roomSize.y / gridSize.y, roomSize.z / gridSize.z };

	if (coord.side == RoomSide::LeftWall)
	{
		return ldCorner + glm::vec3{ 0.0f, divisions.y * coord.pos.y, divisions.z * coord.pos.x };
	}
	else if (coord.side == RoomSide::CenterWall)
	{
		return ldCorner + glm::vec3{ divisions.x * coord.pos.x, divisions.y * coord.pos.y, roomSize.z };
	}
	else if (coord.side == RoomSide::RightWall)
	{
		return ldCorner + glm::vec3{ roomSize.x, divisions.y * coord.pos.y, divisions.z * (gridSize.z - coord.pos.x) };
	}
	else if (coord.side == RoomSide::DownWall)
	{
		return ldCorner + glm::vec3{ divisions.x * coord.pos.x, 0.0f, divisions.z * coord.pos.y };
	}
	else if (coord.side == RoomSide::TopWall)
	{
		return ldCorner + glm::vec3{ divisions.x * coord.pos.x, roomSize.z, divisions.z * coord.pos.y };
	}
	throw;
}

//TODO remove offset
void AddObjectInRoom(RoomSide side, glm::ivec2 pos, glm::ivec2 size, RoomVisual& visual, RenderMesh& mesh, const glm::vec2& offset) 
{
	glm::vec2 ld = -offset + glm::vec2(BoxToPyramid(FlatCoordinateToSpace({ side, pos }), visual.CenterPos));
	glm::vec2 rd = -offset + glm::vec2(BoxToPyramid(FlatCoordinateToSpace({ side, pos + glm::ivec2{ size.x, 0} }), visual.CenterPos));
	glm::vec2 ru = -offset + glm::vec2(BoxToPyramid(FlatCoordinateToSpace({ side, pos + size }), visual.CenterPos));
	glm::vec2 lu = -offset + glm::vec2(BoxToPyramid(FlatCoordinateToSpace({ side, pos + glm::ivec2{ 0, size.y } }), visual.CenterPos));

	std::vector<Vertex2D> points = {
		{ {ld, 0.0f}, {0.0f, 0.0f} },
		{ {rd, 0.0f}, {1.0f, 0.0f} },
		{ {ru, 0.0f}, {1.0f, 1.0f} },
		{ {lu, 0.0f}, {0.0f, 1.0f} }
	};
	mesh.RenderedMesh = GLMesh(points, std::vector<GLuint>({ 0, 1, 2, 0, 2, 3}));
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

		auto [visual] = *em.GetComponents<RoomVisual>();

		int  i = 0;
		for (auto l = em.GetComponents<Transform, DoorData, NewPlacedObjectTag, RenderMesh>(); !l.end(); ++l)
		{
			auto [tr, door, tag, mesh] = *l;

			auto flatCoord = data.rooms[traveler.CurrentRoom].doorData[i].pos;
			glm::vec2 pos = BoxToPyramid(FlatCoordinateToSpace(flatCoord), visual.CenterPos);
			tr.Position = glm::vec3(pos, 1.0f);

			AddObjectInRoom(flatCoord.side, flatCoord.pos, data.rooms[traveler.CurrentRoom].doorData[i].size, visual, mesh, pos);

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