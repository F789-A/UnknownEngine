#pragma once

#include "ecs_EntityManager.h"

#include "GLMesh.h"
#include "GLMaterial.h"
#include "SharedGraphicsResources.h"

struct RenderMesh : public ecs::Component<RenderMesh>
{
	GLMesh RenderedMesh;
	GLMaterial RenderMaterial;

	static void Load(ecs::EntityManager& em, int a, std::map<std::string, std::string>& res)
	{
		Singleton<SharedGraphicsResources> singlRes;
		RenderMesh& renMesh = em.GetComponent<RenderMesh>(a);
		renMesh.RenderedMesh = GLMesh(singlRes->ModelCont.GetModelRef(res["RenderMesh"]).Meshes[0]);
		renMesh.RenderMaterial = GLMaterial(singlRes->GetMaterial(res["RenderMaterial"]));
	}
};

