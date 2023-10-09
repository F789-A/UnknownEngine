#pragma once
#include "GLMaterial.h"
#include "GLLineMesh.h"
#include "EcsSystem.h"
#include "SharedGraphicsResources.h"
#include "GLMesh.h"
#include "Mesh.h"

struct RenderLine : ecs::Component<RenderLine>
{
	static constexpr std::string_view ComponentName = "RenderLine";

	GLMesh RenderedLine;
	GLMaterial RenderedMaterial;

	static void Load(ecs::EntityManager& em, int a, std::map<std::string, std::string>& res)
	{
		auto& render = em.GetComponent<RenderLine>(a);
		render.RenderedLine = GLMesh();
		Singleton<SharedGraphicsResources> singlRes;
		render.RenderedMaterial = GLMaterial(singlRes->GetMaterial(res["RenderedMaterial"]));
	}

};