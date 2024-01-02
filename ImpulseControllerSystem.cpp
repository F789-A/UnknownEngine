#pragma once
#include "Systems.h"
#include "PhysicalBody.h"
#include "ImpulseController.h"
#include "Input.h"
#include "AppTime.h"

void GameTools::ImpulseControllerSystem(ecs::EntityManager& em)
{
	for (auto l = em.GetComponents<ImpulseController, physics::RigidBody>(); !l.end(); ++l)
	{
		auto [ic, rigidbody] = *l;

		float koef = 10.0f;
		if (Input::GetInstance().GetButton("UseT", Input::PressMode::Repeat))
		{
			ic.impulse += glm::vec2(0.0f, 1.0f) * koef * AppTime::GetDeltaTime();
		}
		if (Input::GetInstance().GetButton("UseG", Input::PressMode::Repeat))
		{
			ic.impulse += glm::vec2(0.0f, -1.0f) * koef * AppTime::GetDeltaTime();
		}
		if (Input::GetInstance().GetButton("UseH", Input::PressMode::Repeat))
		{
			ic.impulse += glm::vec2(1.0f, 0.0f) * koef * AppTime::GetDeltaTime();
		}
		if (Input::GetInstance().GetButton("UseF", Input::PressMode::Repeat))
		{
			ic.impulse += glm::vec2(-1.0f, 0.0f) * koef * AppTime::GetDeltaTime();
		}

		if (Input::GetInstance().GetButton("UseT", Input::PressMode::NotPress)
			&& Input::GetInstance().GetButton("UseG", Input::PressMode::NotPress)
			&& Input::GetInstance().GetButton("UseF", Input::PressMode::NotPress)
			&& Input::GetInstance().GetButton("UseH", Input::PressMode::NotPress))
		{
			rigidbody.velocity += ic.impulse * rigidbody.invMass;
			ic.impulse = glm::vec2(0.0f, 0.0f);
		}
		
	}
}
