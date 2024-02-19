#include "ECS\Systems\Systems.h"
#include "ECS\Components\Game\RoomRedrawer.h"
#include "SerializationSystem\EntityLoader.h"
#include "ECS\Components\Events.h"
#include "ECS\Components\Game\LabyrinthData.h"
#include "ECS\Components\Game\RoomTraveler.h"
#include "ECS\Components\Game\GraphDebugInfo.h"
#include "ECS\Components\Transform.h"
#include "Ecs\Components\RenderMesh.h"
#include "Physics\Shapes.h"

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

		static constexpr glm::vec2 luCorner = { -5.0f, 3.725f };
		static constexpr glm::vec2 ruCorner = { 5.0f, 3.725f };
		static constexpr glm::vec2 rdCorner = { 5.0f, -3.725f };
		static constexpr glm::vec2 ldCorner = { -5.0f, -3.725f };

		physics::Line rightVert{ visual.CenterPos + visual.CenterSize, glm::vec2{0.0f, 1.0f} };
		physics::Line rightUp{ ruCorner, visual.CenterPos - ruCorner };
		physics::Line rightDown{ rdCorner, visual.CenterPos - rdCorner };

		physics::Line leftVert{ visual.CenterPos - visual.CenterSize, glm::vec2{0.0f, 1.0f} };
		physics::Line leftUp{ luCorner, visual.CenterPos - luCorner };
		physics::Line leftDown{ ldCorner, visual.CenterPos - ldCorner };

		glm::vec2 lUp = leftVert.IntersectWith(leftUp).value();
		glm::vec2 rUp = rightVert.IntersectWith(rightUp).value();

		glm::vec2 rDown = rightVert.IntersectWith(rightDown).value();
		glm::vec2 lDown = leftVert.IntersectWith(leftDown).value();

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

		}

	}

}