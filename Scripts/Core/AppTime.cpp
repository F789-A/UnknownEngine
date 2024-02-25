#include "AppTime.h"
#include <GLFW\glfw3.h>
#include "GameLoop.h"


float AppTime::GetTime()
{
	return static_cast<float>(glfwGetTime());
}

float AppTime::GetDeltaTime()
{
	return GameLoop::GetInstance().GetDeltaTime();
}
