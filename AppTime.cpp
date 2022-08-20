#include "AppTime.h"

AppTime& AppTime::GetInstance()
{
	static AppTime instance;
	return instance;
}

float AppTime::GetTime()
{
	return WindowApp::GetInstance().GetTime();
}

float AppTime::GetDeltaTime()
{
	return GameLoop::GetInstance().GetDeltaTime();
}