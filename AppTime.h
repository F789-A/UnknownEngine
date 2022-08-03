#pragma once

#include "EngineCore/WindowApp.h"
#include "GameLoop.h"

class AppTime
{
public:

	static AppTime& GetInstance()
	{
		static AppTime instance;
		return instance;
	}

	float GetTime()
	{
		return WindowApp::GetInstance().GetTime();
	}

	float GetDeltaTime()
	{
		return GameLoop::GetInstance().GetDeltaTime();
	}
};