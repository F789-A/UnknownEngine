#pragma once

#include "WindowApp.h"
#include "GameLoop.h"

class AppTime
{
public:

	static AppTime& GetInstance();

	float GetTime();

	float GetDeltaTime();
};