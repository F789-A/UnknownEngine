#include "AimSystem.h"

#include "ecs_EntityManager.h"
#include "Input.h"
#include "AppTime.h"
#include "AimData.h"
#include "RenderLine.h"
#include "AlienData.h"
#include "RectTransformComponent.h"

#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>

void AimCalc();

void AsteroidHunter::CharacterController()
{
	for (auto l = ECS::DefEcs_().entity.GetComponents<AimData>(); !l.end(); ++l)
	{
		auto [aim] = *l;
		float sens = aim.aimSensitivity * AppTime::GetDeltaTime();
		if (Input::GetInstance().GetKey(GLFW_KEY_Y, Input::PressMode::Press))
		{
			aim.size += sens;
		}
		if (Input::GetInstance().GetKey(GLFW_KEY_H, Input::PressMode::Press))
		{
			aim.size -= sens;
		}
		if (Input::GetInstance().GetKey(GLFW_KEY_RIGHT, Input::PressMode::Press))
		{
			aim.center.x += sens;
		}
		if (Input::GetInstance().GetKey(GLFW_KEY_LEFT, Input::PressMode::Press))
		{
			aim.center.x -= sens;
		}
		if (Input::GetInstance().GetKey(GLFW_KEY_UP, Input::PressMode::Press))
		{
			aim.center.y += sens;
		}
		if (Input::GetInstance().GetKey(GLFW_KEY_DOWN, Input::PressMode::Press))
		{
			aim.center.y -= sens;
		}
		aim.size = aim.size > 0 ? aim.size : 0;

		AimCalc();
	}
	
}

void AimCalc()
{
	for (auto l = ECS::DefEcs_().entity.GetComponents<AimData, RenderLine>(); !l.end(); ++l)
	{
		auto [aim, render] = *l;
		
		std::vector<glm::vec2> corner = { {-1, 1}, {1, 1}, {1, -1}, {-1, -1} };
		std::vector<glm::vec2> quad =
		{
			{ aim.center.x - aim.size, aim.center.x + aim.size * aim.aspect },
			{ aim.center.x + aim.size, aim.center.x + aim.size * aim.aspect },
			{ aim.center.x + aim.size, aim.center.x - aim.size * aim.aspect },
			{ aim.center.x - aim.size, aim.center.x - aim.size * aim.aspect }
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
		render.RenderedLine.Draw(render.RenderedMaterial);
	}
}

void AsteroidHunter::AlienController()
{
	for (auto l = ECS::DefEcs_().entity.GetComponents<AlienData, RectTransform>(); !l.end(); ++l)
	{
		auto [alien, transform] = *l;

	}

}