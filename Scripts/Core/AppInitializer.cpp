#include "AppInitializer.h"
#include "WindowApp.h"
#include "GameLoop.h"
#include "Utils\SafeSingleton.h"
#include "ECS\EcsSystem.h"

void RunApp()
{
	InitializeInstance<ecs::EcsSystem>();

	WindowApp::GetInstance().CreateWindow();
	GameLoop::GetInstance().RunLoop();
	WindowApp::GetInstance().TerminateApp();
}