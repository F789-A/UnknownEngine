#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "ECS\EcsSystem.h"
#include "Graphics\GlMaterial.h"
#include "SerializationSystem\SharedGraphicsResources.h"
#include "Utils\SimpleTextProcessor.h"

struct DoorSpriteStorageTag : public ecs::Component<DoorSpriteStorageTag>
{
	static constexpr std::string_view ComponentName = "DoorSpriteStorageTag";

	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res){}
};

struct DecorationsSpriteStorageTag : public ecs::Component<DecorationsSpriteStorageTag>
{
	static constexpr std::string_view ComponentName = "DecorationsSpriteStorageTag";

	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res) {}
};

struct SpriteStorage : public ecs::Component<SpriteStorage>
{
	static constexpr std::string_view ComponentName = "SpriteStorage";

	std::vector<GLMaterial> Materials;
	std::vector<glm::ivec2> Sizes;

	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
	{
		auto& spriteStorage = em.GetComponent<SpriteStorage>(ent);

		std::vector<std::string> textureNames = TextTools::ReadStringArray(res["Textures"]);
		std::vector<int> sizes = TextTools::ReadIntArray(res["Sizes"]);

		Singleton<SharedGraphicsResources> singlRes;

		for (int i = 0; i < textureNames.size(); ++i)
		{
			spriteStorage.Materials.push_back(singlRes->GetMaterial("Materials/Labyrinth/Sprite"));
			spriteStorage.Materials[i].Textures.at("diffuse") = &singlRes->GetGLTextureRef(textureNames[i]);

			spriteStorage.Sizes.push_back(glm::ivec2(sizes[2*i], sizes[2*i+1]));
		}
	}
};