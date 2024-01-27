#pragma once
#include "ECS\EcsSystem.h"

#include "Graphics\GLMaterial.h"
#include "Graphics\GLLineMesh.h"
#include "Graphics\GLMesh.h"

#include "Assets\Mesh.h"

#include "SerializationSystem\SharedGraphicsResources.h"

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