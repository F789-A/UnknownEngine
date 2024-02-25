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
#include "Core\Input.h"

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

		auto [grid] = *em.GetComponents<GridController>();

		grid.points[0] = lUp;
		grid.points[1] = rUp;
		grid.points[2] = rDown;
		grid.points[3] = lDown;

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
			static constexpr glm::vec2 luCorner = { -5.0f, 3.725f };
			static constexpr glm::vec2 ruCorner = { 5.0f, 3.725f };
			static constexpr glm::vec2 rdCorner = { 5.0f, -3.725f };
			static constexpr glm::vec2 ldCorner = { -5.0f, -3.725f };

			std::vector<Vertex> linePoints;
			std::vector<GLuint> indices;

			int xCount = std::ceil((grid.points[1].x - grid.points[0].x) / visual.FrontBoxSize);
			int yCount = std::ceil((grid.points[0].y - grid.points[3].y) / visual.FrontBoxSize);
			int zCount = std::ceil((grid.points[0] - ruCorner).length() / visual.SideBoxSize);
			
			float xLen = (ruCorner.x - luCorner.x) / xCount;
			float yLen = (ruCorner.y - rdCorner.y) / yCount;
			float zLen = visual.SideBoxSize;

			for (int i = 1; i < xCount; ++i)
			{
				linePoints.push_back({ {luCorner.x + (float)i * xLen, luCorner.y, 0} });

				linePoints.push_back({ {grid.points[0].x + (float)i * visual.FrontBoxSize, grid.points[0].y, 0} });
				linePoints.push_back({ {grid.points[0].x + (float)i * visual.FrontBoxSize, grid.points[3].y, 0} });

				linePoints.push_back({ {ldCorner.x + (float)i * xLen, ldCorner.y, 0} });
			}
			for (int i = 1; i < yCount; ++i)
			{
				linePoints.push_back({ {ldCorner.x, ldCorner.y + (float)i * yLen, 0.0f} });

				linePoints.push_back({ {grid.points[0].x, grid.points[3].y + (float)i * visual.FrontBoxSize, 0.0f} });
				linePoints.push_back({ {grid.points[1].x, grid.points[3].y + (float)i * visual.FrontBoxSize, 0.0f} });

				linePoints.push_back({ {rdCorner.x, rdCorner.y + (float)i * yLen, 0.0f} });
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

			glm::vec2 dir1 = glm::normalize(grid.points[0] - luCorner);
			glm::vec2 dir2 = glm::normalize(grid.points[1] - ruCorner);
			glm::vec2 dir3 = glm::normalize(grid.points[2] - rdCorner);
			glm::vec2 dir4 = glm::normalize(grid.points[3] - ldCorner);

			float aspect = std::abs(dir1.x / dir3.x);
			dir3 *= aspect;
			dir4 *= aspect;

			for (int i = 1; i < zCount + 1; ++i)
			{
				linePoints.push_back({ { luCorner + dir1 * (float)i * zLen, 0.0f } });
				linePoints.push_back({ { ruCorner + dir2 * (float)i * zLen, 0.0f } });
				linePoints.push_back({ { rdCorner + dir3 * (float)i * zLen, 0.0f } });
				linePoints.push_back({ { ldCorner + dir4 * (float)i * zLen, 0.0f } });
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