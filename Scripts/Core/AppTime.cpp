#include "AppTime.h"
#include "GameLoop.h"

float AppTime::GetTime()
{
	return glfwGetTime();
}

float AppTime::GetDeltaTime()
{
	return GameLoop::GetInstance().GetDeltaTime();
}
