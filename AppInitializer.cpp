#include "AppInitializer.h"
#include "WindowApp.h"
#include "GameLoop.h"
#include "SafeSingleton.h"
#include "EcsSystem.h"

void RunApp()
{
	InitializeInstance<ecs::EcsSystem>();

	WindowApp::GetInstance().CreateWindow();
	GameLoop::GetInstance().RunLoop();
	WindowApp::GetInstance().TerminateApp();
}