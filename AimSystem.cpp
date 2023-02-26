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

float Scr2Nor(float scr)
{
	return scr * 2 - 1;
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
		aim.size = aim.size > aim.minSize ? aim.size : aim.minSize;

		static std::vector<glm::vec2> corner = { {-1, 1}, {1, 1}, {1, -1}, {-1, -1} };
		float aspect = 3.0f / 4.0f;
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
			ecs::DefEcs().entity.RemoveEntity(ecs::DefEcs().entity.GetEntity(alien));
		
			SerializationSystem::LoadEntity(ecs::DefEcs().entity, "Scenes\\AlienPrefab.txt");
			//
			continue;
		}
		//shot
		auto [player] = *ecs::DefEcs().entity.GetComponents<AimData>();
		player.Health -= alien.Damage * AppTime::GetDeltaTime();

		// move
		int mode = 1;

		if (mode == 2) //randomer
		{
			if ((int)AppTime::GetTime() % 10 < AppTime::GetDeltaTime() * 2)
			{
				int dirX = std::rand();
				int dirY = std::rand();
				alien.dir = glm::vec2((float)dirX / RAND_MAX, (float)dirY / RAND_MAX);
			}
		}
		else if (mode == 3) // coward
		{ 
			alien.dir = transf.pos - em.GetComponent<RectTransform>(player).pos;
		}

		float dir = (float)std::rand() / RAND_MAX;  // bounder
		if (transf.pos.x > 1)
		{
			alien.dir = glm::vec2(0, dir) - transf.pos;
		} 
		if (transf.pos.x < 0)
		{
			alien.dir = glm::vec2(1, dir) - transf.pos;
		}
		if (transf.pos.y > 1)
		{
			alien.dir = glm::vec2(dir, 0) - transf.pos;
		}
		if (transf.pos.y < 0)
		{
			alien.dir = glm::vec2(dir, 1) - transf.pos;
		}
		alien.dir = glm::normalize(alien.dir);
		float delta = std::min(AppTime::GetDeltaTime(), 0.1f);
		transf.pos += glm::vec2(alien.dir.x * delta, alien.dir.y * delta);
		alien.HealthBar->text = std::to_string(alien.Health);
	}
}