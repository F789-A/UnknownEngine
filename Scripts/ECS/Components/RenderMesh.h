#pragma once

#include "ECS\EcsSystem.h"

#include "Graphics\GLMesh.h"
#include "Graphics\GLMaterial.h"
#include "SerializationSystem\SharedGraphicsResources.h"

struct RenderMesh : public ecs::Component<RenderMesh>
{
	static constexpr std::string_view ComponentName = "RenderMesh";

	GLMesh RenderedMesh;
	GLMaterial RenderMaterial;

	static void Load(ecs::EntityManager& em, int a, std::map<std::string, std::string>& res)
	{
		Singleton<SharedGraphicsResources> singlRes;
		RenderMesh& renMesh = em.GetComponent<RenderMesh>(a);
		auto& mesh = singlRes->ModelCont.GetModelRef(res["RenderMesh"]).Meshes[0];
		renMesh.RenderedMesh = GLMesh(mesh.Vertices, mesh.Indices);
		renMesh.RenderMaterial = GLMaterial(singlRes->GetMaterial(res["RenderMaterial"]));
	}
};

