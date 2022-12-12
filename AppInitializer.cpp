#include "AppInitializer.h"
#include "WindowApp.h"
#include "GameLoop.h"

void RunApp()
{
	WindowApp::GetInstance().CreateWindow();
	GameLoop::GetInstance().RunLoop();
	WindowApp::GetInstance().TerminateApp();
}