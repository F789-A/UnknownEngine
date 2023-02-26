#pragma once
#include "GLMaterial.h"
#include "GLLineMesh.h"
#include "ecs_EntityManager.h"
#include "SharedGraphicsResources.h"

struct RenderLine : ecs::Component<RenderLine>
{
	GLLineMesh RenderedLine;
	GLMaterial RenderedMaterial;

	static void Load(ecs::EntityManager& em, int a, std::map<std::string, std::string>& res)
	{
		auto& render = em.GetComponent<RenderLine>(a);
		std::vector<glm::vec2> arr =
		{
			{0, 0}, {0, 0}
		};
		render.RenderedLine = GLLineMesh(arr);
		Singleton<SharedGraphicsResources> singlRes;
		render.RenderedMaterial = singlRes->GetMaterial("Materials\\fuck.uemat");
	}

};