#include "ECS\Systems\Systems.h"
#include "ECS\Components\Game\GraphDebugInfo.h"
#include "ECS\Components\Game\LabyrinthData.h"
#include "ECS\Components\UiComponents.h"
#include "ECS\Components\Game\RoomTraveler.h"
#include "GameTools\GraphGenerator.h"
#include "Core\Input.h"
#include "SerializationSystem\SharedGraphicsResources.h"


void Labyrinth::GraphDebugger(ecs::EntityManager& em)
{
	for (auto l = em.GetComponents<GraphDebugInfo, LabyrinthData, RoomTraveler>(); !l.end(); ++l)
	{
		auto [debInfo, labyrinth, travaler] = *l;

		if (Input::GetInstance().GetButton("UseQ", Input::PressMode::Press))
		{
			debInfo.ActiveDebug = !debInfo.ActiveDebug;
			if (!debInfo.ActiveDebug)
			{
				for (auto ent : debInfo.Entites)
				{
					em.RemoveEntity(ent);
				}
				debInfo.Entites.clear();
			}
			else
			{
				debInfo.forceRedraw = true;
			}
		}

		if (debInfo.ActiveDebug && debInfo.forceRedraw)
		{
			debInfo.forceRedraw = false;
			for (auto ent : debInfo.Entites)
			{
				em.RemoveEntity(ent);
			}
			debInfo.Entites.clear();
			
			for (int i = 0; i < labyrinth.levelGraph.Verts.size(); ++i)
			{
				auto e = em.AddEntity();
				debInfo.Entites.push_back(e);

				em.AddComponent<RectTransform>(e);
				auto& rt = em.GetComponent<RectTransform>(e);
				rt.pos = glm::vec2((float)labyrinth.levelGraph.debugPos[i].first / 10.0f + 0.5f, (float)labyrinth.levelGraph.debugPos[i].second / 10.0f + 0.1f);
				rt.size = glm::vec3(0.05, 0.05, 0);
				rt.priority = 1;

				em.AddComponent<Image>(e);
				auto& im = em.GetComponent<Image>(e);
				
				Singleton<SharedGraphicsResources> shr;

				if (travaler.CurrentRoom == i)
				{
					im.Material = GLMaterial(shr->GetMaterial("Materials\\Labyrinth\\GraphPoint2.txt"));
				}
				else
				{
					im.Material = GLMaterial(shr->GetMaterial("Materials\\Labyrinth\\GraphPoint.txt"));
				}

				auto line = em.AddEntity();
				debInfo.Entites.push_back(line);
				em.AddComponent<RenderLine>(line);
				em.AddComponent<RectTransform>(line);

				auto& lineRenderer = em.GetComponent<RenderLine>(line);

				lineRenderer.RenderedMaterial = GLMaterial(shr->GetMaterial("Materials/Simple.uemat"));

				std::vector<GLuint> verts;
				std::vector<Vertex> arr;
				arr.push_back({ { (float)labyrinth.levelGraph.debugPos[i].first / 10.0f * 2.0f, (float)labyrinth.levelGraph.debugPos[i].second / 10.0f * 2.0f - 0.8f, 0 } });
				int f = 1;
				for (auto j : labyrinth.levelGraph.Verts[i])
				{
					arr.push_back({ { (float)labyrinth.levelGraph.debugPos[j].first / 10.0f * 2.0f, (float)labyrinth.levelGraph.debugPos[j].second / 10.0f * 2.0f - 0.8f, 0 } });
					verts.push_back(0);
					verts.push_back(f);
					f++;
				}
				lineRenderer.RenderedLine = GLMesh(arr, verts, GLMesh::GeometryTypes::Lines);
			}
		}
	}
}