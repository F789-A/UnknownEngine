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

enum SpriteSide
{
	LeftWall	= 0b0000001,
	CenterWall	= 0b0000010,
	RightWall	= 0b0000100,
	DownWall	= 0b0001000,
	TopWall		= 0b0010000,
	Plinth		= 0b0100000,
	Eave		= 0b1000000
};

struct SpriteStorage : public ecs::Component<SpriteStorage>
{
	static constexpr std::string_view ComponentName = "SpriteStorage";

	std::vector<GLMaterial> Materials;
	std::vector<glm::ivec2> Sizes;
	std::vector<SpriteSide> Sides;

	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
	{
		auto& spriteStorage = em.GetComponent<SpriteStorage>(ent);

		std::vector<std::string> textureNames = TextTools::ReadStringArray(res["Textures"]);
		std::vector<int> sizes = TextTools::ReadIntArray(res["Sizes"]);
		std::vector<std::string> sides = TextTools::ReadStringArray(res["Sides"]);

		Singleton<SharedGraphicsResources> singlRes;

		for (int i = 0; i < textureNames.size(); ++i)
		{
			spriteStorage.Materials.push_back(singlRes->GetMaterial("Materials/Labyrinth/Sprite.txt"));
			spriteStorage.Materials[i].Textures.at("diffuse") = &singlRes->GetGLTextureRef(textureNames[i]);

			spriteStorage.Sizes.push_back(glm::ivec2(sizes[2*i], sizes[2*i+1]));

			SpriteSide side;
			for (auto l : sides[i])
			{
				if (l == 'l')
				{
					side = static_cast<SpriteSide>(side | LeftWall);
				} 
				else if (l == 'c')
				{
					side = static_cast<SpriteSide>(side | CenterWall);
				}
				else if(l == 'r')
				{
					side = static_cast<SpriteSide>(side | RightWall);
				}
				else if(l == 'd')
				{
					side = static_cast<SpriteSide>(side | DownWall);
				}
				else if(l == 't')
				{
					side = static_cast<SpriteSide>(side | TopWall);
				}
				else if(l == 'p')
				{
					side = static_cast<SpriteSide>(side | Plinth);
				}
				else if(l == 'e')
				{
					side = static_cast<SpriteSide>(side | Eave);
				}
			}

			spriteStorage.Sides.push_back(side);
		}
	}
};