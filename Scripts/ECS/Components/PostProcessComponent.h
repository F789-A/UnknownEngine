#pragma once
#include "ECS\EcsSystem.h"
#include "Graphics\GLMaterial.h"
#include "SerializationSystem\SharedGraphicsResources.h"

struct PostProcessComponent : public ecs::Component<PostProcessComponent>
{
	static constexpr std::string_view ComponentName = "PostProcessComponent";

	bool IsEnabled = false;
	GLMaterial RenderedMaterial;

	static void Load(ecs::EntityManager& em, int a, std::map<std::string, std::string>& res)
	{
		Singleton<SharedGraphicsResources> singlRes;
		PostProcessComponent& process = em.GetComponent<PostProcessComponent>(a);
		process.RenderedMaterial = GLMaterial(singlRes->GetMaterial(res.at("RenderedMaterial")));
	}
};