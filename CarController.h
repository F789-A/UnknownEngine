#pragma once
#include "Transform.h"
#include "Component.h"
#include "ILoopUpdate.h"
#include "Input.h"
#include "AppTime.h"

class CarController: public ec::Component, public ILoopUpdate<UpdateType::GameLoop>
{
	void Update()
	{
		Transform& transf = GetLinkedEntity().GetComponent<Transform>();
		if (Input::GetInstance().GetButton("Front", 1))
		{
			transf.Position += transf.Front() * AppTime::GetDeltaTime();
		}
	}

};