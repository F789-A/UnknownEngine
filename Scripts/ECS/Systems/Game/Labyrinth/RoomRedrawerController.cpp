#include "ECS\Systems\Systems.h"
#include <glm/glm.hpp>
#include "ECS\Components\Game\Labyrinth\RoomRedrawer.h"
#include "SerializationSystem\EntityLoader.h"
#include "ECS\Components\Events.h"
#include "ECS\Components\Game\Labyrinth\LabyrinthData.h"
#include "ECS\Components\Game\Labyrinth\RoomTraveler.h"
#include "ECS\Components\Transform.h"
#include "Ecs\Components\RenderMesh.h"
#include "ECS\Components\Physics\PhysicalBody.h"
#include "ECS\Components\Physics\Collider.h"
#include "Physics\Shapes.h"
#include "Core\Input.h"

glm::vec3 BoxToPyramid(const glm::vec3& pos, const glm::vec3& peak)
{
	return pos.z / peak.z * peak + (1.0f - pos.z / peak.z) * pos;
}

glm::vec3 FlatCoordinateToSpace(const FlatCoord& coord, const RoomVisual& visual)
{
	const glm::vec3& roomSize = visual.RoomSize;
	glm::ivec3 gridSize{ visual.BoxXCount, visual.BoxYCount, visual.BoxZCount };

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

void AddObjectInRoom(RoomSide side, glm::ivec2 pos, glm::ivec2 size, RoomVisual& visual, RenderMesh& mesh, physics::Collider* coll)
{
	glm::vec2 ld = glm::vec2(BoxToPyramid(FlatCoordinateToSpace({ side, pos }, visual), visual.CenterPos));
	glm::vec2 rd = glm::vec2(BoxToPyramid(FlatCoordinateToSpace({ side, pos + glm::ivec2{ size.x, 0} }, visual), visual.CenterPos));
	glm::vec2 ru = glm::vec2(BoxToPyramid(FlatCoordinateToSpace({ side, pos + size }, visual), visual.CenterPos));
	glm::vec2 lu = glm::vec2(BoxToPyramid(FlatCoordinateToSpace({ side, pos + glm::ivec2{ 0, size.y } }, visual), visual.CenterPos));

	std::vector<Vertex2D> points = {
		{ {ld, 0.0f}, {0.0f, 0.0f} },
		{ {rd, 0.0f}, {1.0f, 0.0f} },
		{ {ru, 0.0f}, {1.0f, 1.0f} },
		{ {lu, 0.0f}, {0.0f, 1.0f} }
	};
	mesh.RenderedMesh = GLMesh(points, std::vector<GLuint>({ 0, 1, 2, 0, 2, 3}));

	if (coll)
		coll->shape = std::make_unique<physics::Polygon>(std::vector<glm::vec2>{ ld , rd, ru, lu });
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

		auto [visual] = *em.GetComponents<RoomVisual>();

		const auto& currentRoom = data.rooms[traveler.CurrentRoom];

		//if (== 0)
		//{
			// start
		//}

		if (currentRoom.isWin)
		{
			SerializationSystem::LoadEntity(em, "Scenes\\Labyrinth\\WinDoorPrefab.txt");
			for (auto l = em.GetComponents<NewPlacedObjectTag, WinDoor, Transform, RenderMesh, physics::Collider>(); !l.end(); ++l)
			{
				auto [tag, door, tr , mesh, coll] = *l;

				tr.Position = glm::vec3(0.0f, 0.0f, 1.0f);

				AddObjectInRoom(RoomSide::CenterWall, { 2, 0 }, {2, 2}, visual, mesh, &coll);

				redrawer.Entites.push_back(em.GetEntity(tag));
				em.RemoveComponent<NewPlacedObjectTag>(em.GetEntity(tag));
			}
		}

		for (int i = 0; i < currentRoom.doorData.size(); ++i)
		{
			SerializationSystem::LoadEntity(em, "Scenes\\Labyrinth\\DoorPrefab.txt");
		}

		int  i = 0;
		for (auto l = em.GetComponents<Transform, Door, NewPlacedObjectTag, RenderMesh, physics::Collider>(); !l.end(); ++l)
		{
			auto [tr, door, tag, mesh, coll] = *l;

			const auto& doorData = currentRoom.doorData[i];

			tr.Position = glm::vec3(0.0f, 0.0f, 1.0f);

			AddObjectInRoom(doorData.coord.side, doorData.coord.pos, doorData.size, visual, mesh, &coll);

			door.NextRoom = data.levelGraph.Verts[traveler.CurrentRoom][i];

			Singleton<SharedGraphicsResources> singlRes;
			if (traveler.PreviousRoom == door.NextRoom)
			{
				GLMaterial material = doorData.material;
				material.Textures.at("diffuse") = &singlRes->GetGLTextureRef("Textures/Labyrinth/DoorOpen.png");
				mesh.RenderMaterial = material;
			}
			else
			{
				mesh.RenderMaterial = doorData.material;
			}

			i++;

			redrawer.Entites.push_back(em.GetEntity(tag));
			em.RemoveComponent<NewPlacedObjectTag>(em.GetEntity(tag));
		}

		for (int j = 0; j < currentRoom.decorData.size(); ++j)
		{
			SerializationSystem::LoadEntity(em, "Scenes\\Labyrinth\\DecorPrefab.txt");
		}

		int j = 0;
		for (auto l = em.GetComponents<NewPlacedObjectTag, Transform, RenderMesh>(); !l.end(); ++l)
		{
			auto [tag, tr, mesh] = *l;

			const auto& decorData = currentRoom.decorData[j];

			tr.Position = glm::vec3(0.0f, 0.0f, 1.0f);

			AddObjectInRoom(decorData.coord.side, decorData.coord.pos, decorData.size, visual, mesh, nullptr);
			mesh.RenderMaterial = decorData.material;

			j++;

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

		glm::vec2 luCorner{ -visual.RoomSize.x / 2.0f, visual.RoomSize.y / 2.0f };
		glm::vec2 ruCorner{ visual.RoomSize.x / 2.0f, visual.RoomSize.y / 2.0f};
		glm::vec2 rdCorner{ visual.RoomSize.x / 2.0f, -visual.RoomSize.y / 2.0f};
		glm::vec2 ldCorner{ -visual.RoomSize.x / 2.0f, -visual.RoomSize.y / 2.0f};

		glm::vec2 lUp = BoxToPyramid(glm::vec3(luCorner, visual.RoomSize.z), visual.CenterPos);
		glm::vec2 rUp = BoxToPyramid(glm::vec3(ruCorner, visual.RoomSize.z), visual.CenterPos);
		glm::vec2 rDown = BoxToPyramid(glm::vec3(rdCorner, visual.RoomSize.z), visual.CenterPos);
		glm::vec2 lDown = BoxToPyramid(glm::vec3(ldCorner, visual.RoomSize.z), visual.CenterPos);

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

			glm::vec2 luCorner{ -visual.RoomSize.x / 2.0f, visual.RoomSize.y / 2.0f };
			glm::vec2 ruCorner{ visual.RoomSize.x / 2.0f, visual.RoomSize.y / 2.0f };
			glm::vec2 rdCorner{ visual.RoomSize.x / 2.0f, -visual.RoomSize.y / 2.0f };
			glm::vec2 ldCorner{ -visual.RoomSize.x / 2.0f, -visual.RoomSize.y / 2.0f };

			std::vector<Vertex> linePoints;
			std::vector<GLuint> indices;
			
			float xLen = visual.RoomSize.x / visual.BoxXCount;
			float yLen = visual.RoomSize.y / visual.BoxYCount;
			float zLen = visual.RoomSize.z / visual.BoxZCount;

			for (int i = 1; i < visual.BoxXCount; ++i)
			{
				float xCoord = luCorner.x + (float)i * xLen;
				linePoints.push_back({ {xCoord, luCorner.y, 0.0f} });
				linePoints.push_back({ {glm::vec2(BoxToPyramid({ xCoord, luCorner.y, visual.RoomSize.z}, visual.CenterPos)), 0.0f} });
				linePoints.push_back({ {glm::vec2(BoxToPyramid({ xCoord, ldCorner.y, visual.RoomSize.z}, visual.CenterPos)), 0.0f} });
				linePoints.push_back({ {xCoord, ldCorner.y, 0.0f} });
			}
			for (int i = 1; i < visual.BoxYCount; ++i)
			{
				float yCoord = ldCorner.y + (float)i * yLen;
				linePoints.push_back({ {ldCorner.x, yCoord, 0.0f} });
				linePoints.push_back({ {glm::vec2(BoxToPyramid({ ldCorner.x, yCoord, visual.RoomSize.z}, visual.CenterPos)), 0.0f} });
				linePoints.push_back({ {glm::vec2(BoxToPyramid({ rdCorner.x, yCoord, visual.RoomSize.z}, visual.CenterPos)), 0.0f} });
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