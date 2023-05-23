#include "AppInitializer.h"
#include "WindowApp.h"
#include "GameLoop.h"
#include "SafeSingleton.h"
#include "TagController.h"
#include "EcsSystem.h"

void RunApp()
{
	InitializeInstance<ecs::EcsSystem>();

	InitializeInstance<TagController>();
	InstanceOf<TagController>().Bind(InstanceOf<ecs::EcsSystem>().entity);

	WindowApp::GetInstance().CreateWindow();
	GameLoop::GetInstance().RunLoop();
	WindowApp::GetInstance().TerminateApp();
}