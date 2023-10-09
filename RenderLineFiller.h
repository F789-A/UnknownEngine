#pragma once
#include "EcsSystem.h"
#include "RenderLine.h"

struct RenderLineFiller : public ecs::Component<RenderLineFiller>
{
	static constexpr std::string_view ComponentName = "RenderLineFiller";
	
	static void Load(ecs::EntityManager& em, int a, std::map<std::string, std::string>& res)
	{
		auto& filler = em.GetComponent<RenderLineFiller>(a);
		auto& render = em.GetComponent<RenderLine>(em.GetEntity(filler));
		std::vector<Vertex> vert = { {{0.0f, 0.0f, 0.0f}}, {{1.0f, 0.0f, 0.0f}} };
		render.RenderedLine = GLMesh(vert, {0, 1}, GLMesh::GeometryTypes::Lines);
	}
};