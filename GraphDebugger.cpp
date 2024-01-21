#include "Systems.h"
#include "GraphDebugInfo.h"
#include "Input.h"
#include "GraphGenerator.h"
#include "UiComponents.h"
#include "RenderLine.h"
#include "SharedGraphicsResources.h"

void HellishLabyrinth::GraphDebugger(ecs::EntityManager& em)
{
	for (auto l = em.GetComponents<GraphDebugInfo>(); !l.end(); ++l)
	{
		auto [lvl] = *l;
		if (Input::GetInstance().GetButton("UseQ", Input::PressMode::Press))
		{
			Graph g{34580, 8, 4, 4, 8, 4 };
			
			for (int i = 0; i < g.Verts.size(); ++i)
			{
				auto e = em.AddEntity();

				em.AddComponent<RectTransform>(e);
				auto& rt = em.GetComponent<RectTransform>(e);
				rt.pos = glm::vec2((float)g.debugPos[i].first / 10.0f + 0.5f, (float)g.debugPos[i].second / 10.0f + 0.1f);
				rt.size = glm::vec3(0.05, 0.05, 0);
				rt.priority = 1;

				em.AddComponent<Image>(e);
				auto& im = em.GetComponent<Image>(e);
				
				Singleton<SharedGraphicsResources> shr;

				im.Material = GLMaterial(shr->GetMaterial("Materials\\GraphPoint.txt"));

				auto line = em.AddEntity();
				em.AddComponent<RenderLine>(line);

				auto& lineRenderer = em.GetComponent<RenderLine>(line);

				lineRenderer.RenderedMaterial = GLMaterial(shr->GetMaterial("Materials/Simple.uemat"));

				std::vector<GLuint> verts;
				std::vector<Vertex> arr;
				arr.push_back({ { (float)g.debugPos[i].first / 10.0f * 2.0f, (float)g.debugPos[i].second / 10.0f * 2.0f - 0.8f, 0 } });
				int f = 1;
				for (auto j : g.Verts[i])
				{
					arr.push_back({ { (float)g.debugPos[j].first / 10.0f * 2.0f, (float)g.debugPos[j].second / 10.0f * 2.0f - 0.8f, 0 } });
					verts.push_back(0);
					verts.push_back(f);
					f++;
				}
				lineRenderer.RenderedLine = GLMesh(arr, verts, GLMesh::GeometryTypes::Lines);
			}
		}
	}
}