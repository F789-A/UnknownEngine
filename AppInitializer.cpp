#include "AppInitializer.h"

void RunApp()
{
	WindowApp::GetInstance().CreateWindow();
	GameLoop::GetInstance().RunLoop();
	WindowApp::GetInstance().TerminateApp();
}