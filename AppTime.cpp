#include "AppTime.h"
#include "GameLoop.h"

_AppTime& _AppTime::GetInstance()
{
	static _AppTime instance;
	return instance;
}

float _AppTime::GetTime()
{
	return glfwGetTime();
}

float _AppTime::GetDeltaTime()
{
	return GameLoop::GetInstance().GetDeltaTime();
}

float AppTime::GetTime()
{
	return glfwGetTime();
}

float AppTime::GetDeltaTime()
{
	return GameLoop::GetInstance().GetDeltaTime();
}
