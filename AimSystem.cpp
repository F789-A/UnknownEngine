#include "Systems.h"

#include "EcsSystem.h"
#include "Input.h"
#include "AppTime.h"
#include "AimData.h"
#include "RenderLine.h"
#include "AlienData.h"
#include "UiComponents.h"

#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include  <algorithm>
#include "GLMaterial.h"
#include <glm/glm.hpp>

#include "EntityLoader.h"
#include "LevelData.h"
#include "TagController.h"
#include "SafeSingleton.h"

float Scr2Nor(float scr)
{
	return scr * 2 - 1;
}

glm::vec2 ClampVec2(glm::vec2 v, const glm::vec2& min, const glm::vec2& max)
{
	return { std::clamp(v.x, min.x, max.x), std::clamp(v.y, min.y, max.y) };
}

void AsteroidHunter::CharacterController(ecs::EntityManager& em)
{
	for (auto l = em.GetComponents<AimData, RenderLine>(); !l.end(); ++l)
	{
		auto [aim, render] = *l;
		float sens = aim.aimSensitivity * AppTime::GetDeltaTime();
		if (Input::GetInstance().GetButton("Expansion", Input::PressMode::Repeat))
		{
			aim.size += sens;
		}
		if (Input::GetInstance().GetButton("Contraction", Input::PressMode::Repeat))
		{
			aim.size -= sens;
		}
		if (Input::GetInstance().GetButton("Front", Input::PressMode::Repeat))
		{
			aim.center.y += sens * aim.Speed;
		}
		if (Input::GetInstance().GetButton("Backward", Input::PressMode::Repeat))
		{
			aim.center.y -= sens * aim.Speed;
		}
		if (Input::GetInstance().GetButton("Right", Input::PressMode::Repeat))
		{
			aim.center.x += sens * aim.Speed;
		}
		if (Input::GetInstance().GetButton("Left", Input::PressMode::Repeat))
		{
			aim.center.x -= sens * aim.Speed;
		}

		aim.size = std::clamp(aim.size, aim.minSize, aim.maxSize);

		float aspect = 3.0f / 4.0f;
		glm::vec2 boundCorner = { 1 - (aim.size + 0.01) * aspect , 1 - (aim.size+0.2) * aspect };
		aim.center = ClampVec2(aim.center, -boundCorner, boundCorner);

		static std::vector<glm::vec2> corner = { {-1, 1}, {1, 1}, {1, -1}, {-1, -1} };

		std::vector<glm::vec2> quad =
		{
			{ aim.center.x - aim.size * aspect, aim.center.y + aim.size },
			{ aim.center.x + aim.size * aspect, aim.center.y + aim.size },
			{ aim.center.x + aim.size * aspect, aim.center.y - aim.size },
			{ aim.center.x - aim.size * aspect, aim.center.y - aim.size }
		};

		std::vector<glm::vec2> arr =
		{
			//
			quad[0], quad[1],
			quad[1], quad[2],
			quad[2], quad[3],
			quad[3], quad[0],
			//
			corner[0], quad[0],
			corner[1], quad[1],
			corner[2], quad[2],
			corner[3], quad[3]
		};
		render.RenderedLine.setVert(arr);

		for (auto k = em.GetComponents<AlienData, RectTransform>(); !k.end(); ++k)
		{
			auto [alien, transf] = *k;
			if (aim.center.x - aim.size * aspect < Scr2Nor(transf.pos.x) && Scr2Nor(transf.pos.x) < aim.center.x + aim.size * aspect
				&& aim.center.y - aim.size < Scr2Nor(transf.pos.y) && Scr2Nor(transf.pos.y) < aim.center.y + aim.size)
			{
				alien.Health -= aim.Damage * aim.minSize/aim.size * AppTime::GetDeltaTime();
			}
		}
		aim.text->text = std::string("Health: ") + std::to_string((int)aim.Health);
	}
}

void AsteroidHunter::AlienController(ecs::EntityManager& em)
{
	for (auto l = em.GetComponents<AlienData, RectTransform>(); !l.end(); ++l)
	{
		auto [alien, transf] = *l;

		//died
		if (alien.Health <= 0)
		{
			ecs::DefEcs().entity.RemoveEntity(ecs::DefEcs().entity.GetEntity(*transf.childs[0]));
			ecs::DefEcs().entity.RemoveEntity(ecs::DefEcs().entity.GetEntity(alien));

			auto& levelData = em.GetComponent<LevelData>(InstanceOf<TagController>().GetEntityWithTag("LevelData"));
			levelData.countAlien--;
			//SerializationSystem::LoadEntity(ecs::DefEcs().entity, "Scenes\\AlienPrefab.txt");
			// Добавить плавное появление из-за границ экрана
			continue;
		}
		//shot
		auto [player] = *ecs::DefEcs().entity.GetComponents<AimData>();
		player.Health -= alien.Damage * AppTime::GetDeltaTime();

		alien.HealthBar->text = std::to_string(alien.Health);

		alien.Timer -= AppTime::GetDeltaTime();

		if (alien.Timer < 0)
		{
			alien.Timer = alien.TimeToChangeDir;
			if (static_cast<bool>(alien.alienType & AlienTypes::Randomer))
			{
				int dirX = std::rand();
				int dirY = std::rand();
				alien.dir = glm::vec2((float)dirX / RAND_MAX, (float)dirY / RAND_MAX);
			}
			else if (static_cast<bool>(alien.alienType & AlienTypes::Coward))
			{
				alien.dir = transf.pos - player.center;
			}
			alien.dir = glm::normalize(alien.dir);
		}
		if (static_cast<bool>(alien.alienType & AlienTypes::PingPonger))
		{
			float eps = (float)std::rand() / RAND_MAX / 10.0;
			if ((transf.pos.x > 1 && alien.dir.x > 0) || (transf.pos.x < 0 && alien.dir.x < 0))
			{
				alien.dir = glm::normalize(glm::vec2(-alien.dir.x + eps, alien.dir.y));
			} 
			if ((transf.pos.y > 1 && alien.dir.y > 0) || (transf.pos.y < 0 && alien.dir.y < 0))
			{
				alien.dir = glm::normalize(glm::vec2(alien.dir.x, -alien.dir.y + eps));
			}
		}
		else if (static_cast<bool>(alien.alienType & AlienTypes::Bounder))
		{
			float rndDir = ((float)std::rand() / RAND_MAX -0.5) * 10;
			if (transf.pos.x > 1)
			{
				alien.dir = glm::normalize(glm::vec2(-1, rndDir));
			}
			if (transf.pos.x < 0)
			{
				alien.dir = glm::normalize(glm::vec2(1, rndDir));
			}
			if (transf.pos.y > 1)
			{
				alien.dir = glm::normalize(glm::vec2(rndDir, -1));
			}
			if (transf.pos.y < 0 )
			{
				alien.dir = glm::normalize(glm::vec2(rndDir, 1));
			}
		}
		alien.dir = glm::normalize(alien.dir);
		float delta = std::min(AppTime::GetDeltaTime(), 0.1f);
		transf.pos += glm::vec2(alien.dir.x * delta, alien.dir.y * delta);
	}
}