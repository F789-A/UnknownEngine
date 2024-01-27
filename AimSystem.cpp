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
#include "SafeSingleton.h"
#include "GraphicCore.h"
#include <random>

std::vector<GLfloat> ToFloat3Array(const std::vector<glm::vec2>& source)
{
	std::vector<GLfloat> res;
	res.reserve(source.size() * 3);
	for (int i = 0; i < source.size(); i++)
	{
		res.push_back(source[i].x);
		res.push_back(source[i].y);
		res.push_back(0);
	}
	return res;
}

float Scr2Nor(float scr)
{
	return scr * 2 - 1;
}

glm::vec2 ClampVec2(glm::vec2 v, const glm::vec2& min, const glm::vec2& max)
{
	return { std::clamp(v.x, min.x, max.x), std::clamp(v.y, min.y, max.y) };
}

static glm::vec2 right = { 1.0f, 0.0f };
static glm::vec2 left =  {-1.0f, 0.0f };
static glm::vec2 up =	 { 0.0f, 1.0f };
static glm::vec2 down =	 { 0.0f, -1.0f};

void ProcessShootEffect(ecs::EntityManager& em, const std::vector<glm::vec2>& linePoints)
{
	for (auto l = em.GetComponents<AimEffect, RenderLine>(); !l.end(); ++l)
	{
		auto [effect, effectRender] = *l;

		static bool onceEff = true;
		if (onceEff)
		{
			onceEff = false;
			std::vector<Vertex> arr;
			for (int i = 0; i < 40; i++)
			{
				arr.push_back(Vertex{ { 0.0, 0.0, 0.0 } });
			}
			std::vector<GLuint> indices;
			for (int i = 0; i < 40; i++)
			{
				indices.push_back(i);
			}
			GLMesh b(arr, indices, GLMesh::GeometryTypes::Lines);
			effectRender.RenderedLine = std::move(b);
		}

		const glm::vec2& leftDownCorner = linePoints[3];
		const glm::vec2& rightUpCorner = linePoints[1];
		float distX = rightUpCorner.x - leftDownCorner.x;
		float distY = rightUpCorner.y - leftDownCorner.y;

		static std::mt19937 rnd(0);
		std::uniform_real_distribution<float> distBig(0.05f, 0.95);
		std::uniform_real_distribution<float> distSmall(0, 0.1f);
		std::uniform_real_distribution<float> distLen(0.1f, 0.15f);

		const float tolerance = 0.05;

		const float len = 0.05f;

		auto updatePos = [&](int num)
		{
			bool tailReached = glm::length(effect.Lines[num].TargetPos - effect.Lines[num].CurrentPos) < tolerance;
			bool headReached = glm::length(effect.Lines[num + 1].TargetPos - effect.Lines[num + 1].CurrentPos) < tolerance;
			if (tailReached && headReached)
			{
				effect.Lines[num].CurrentPos = { distSmall(rnd), distBig(rnd) };
				effect.Lines[num].TargetPos = effect.Lines[num].CurrentPos + right * distLen(rnd);
				effect.Lines[num + 1].CurrentPos = effect.Lines[num].CurrentPos + right * len;
				effect.Lines[num + 1].TargetPos = effect.Lines[num].TargetPos + right * len;
			}
			glm::vec2 dir = glm::normalize(effect.Lines[num].TargetPos - effect.Lines[num].CurrentPos) * AppTime::GetDeltaTime();
			if (!tailReached)
			{
				effect.Lines[num].CurrentPos += dir * effect.TailSpeed;
			}
			if (!headReached)
			{
				effect.Lines[num + 1].CurrentPos += dir * effect.HeadSpeed;
			}
		};


		auto swapXY = [](const glm::vec2& vec)
		{
			return glm::vec2(vec.y, vec.x);
		};

		const glm::vec2 vecMult = { distX, distY };
		std::vector<glm::vec2> effArr;
		for (int i = 0; i < 5; i++)
		{
			updatePos(i * 2);
			effArr.push_back(leftDownCorner + effect.Lines[i * 2].CurrentPos * vecMult);
			effArr.push_back(leftDownCorner + effect.Lines[i * 2 + 1].CurrentPos * vecMult);

			updatePos(10 + i * 2);
			effArr.push_back(rightUpCorner + effect.Lines[10 + i * 2].CurrentPos * -1.0f * vecMult);
			effArr.push_back(rightUpCorner + effect.Lines[10 + i * 2 + 1].CurrentPos * -1.0f * vecMult);

			updatePos(20 + i * 2);
			effArr.push_back(leftDownCorner + swapXY(effect.Lines[20 + i * 2].CurrentPos) * vecMult);
			effArr.push_back(leftDownCorner + swapXY(effect.Lines[20 + i * 2 + 1].CurrentPos) * vecMult);

			updatePos(30 + i * 2);
			effArr.push_back(rightUpCorner + swapXY(effect.Lines[30 + i * 2].CurrentPos) * -1.0f * vecMult);
			effArr.push_back(rightUpCorner + swapXY(effect.Lines[30 + i * 2 + 1].CurrentPos) * -1.0f * vecMult);
		}

		std::vector<GLfloat> effArr_1 = ToFloat3Array(effArr);
		effectRender.RenderedLine.SetVertexData(effArr_1);
	}
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

		float aspect = GraphicCore::GetInstance().Aspect;
		glm::vec2 boundCorner = { 1 - (aim.size + 0.01f) * aspect , 1 - (aim.size+0.2f) * aspect };
		aim.center = ClampVec2(aim.center, -boundCorner, boundCorner);


		std::vector<glm::vec2> linePoints =
		{
			{ aim.center.x - aim.size * aspect, aim.center.y + aim.size },
			{ aim.center.x + aim.size * aspect, aim.center.y + aim.size },
			{ aim.center.x + aim.size * aspect, aim.center.y - aim.size },
			{ aim.center.x - aim.size * aspect, aim.center.y - aim.size },
			{-1, 1}, {1, 1}, {1, -1}, {-1, -1}
		};
		
		static bool once = true;
		if (once)
		{
			once = false;

			std::vector<Vertex> arr;
			for (auto l : linePoints)
			{
				arr.push_back({ { l.x, l.y, 0 } });
			}
			GLMesh b(arr, std::vector<GLuint>({ 0, 1, 1, 2, 2, 3, 3, 0, 4, 0, 5, 1, 6, 2, 7, 3 }),
				GLMesh::GeometryTypes::Lines);
			render.RenderedLine = std::move(b);
		}
		std::vector<GLfloat> arr;
		for (int i = 0; i < 4; i++)
		{
			arr.push_back(linePoints[i].x);
			arr.push_back(linePoints[i].y);
			arr.push_back(0);
		}
		render.RenderedLine.SetVertexData(arr);

		ProcessShootEffect(em, linePoints);
		
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
			em.RemoveEntity(em.GetEntity(*transf.childs[0]));
			em.RemoveEntity(em.GetEntity(alien));

			for (auto ll = em.GetComponents<LevelData>(); !ll.end(); ++ll)
			{
				auto [levelData] = *ll;
				levelData.countAlien--;
			}
			continue;
		}
		//shot
		auto [player] = *em.GetComponents<AimData>();
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