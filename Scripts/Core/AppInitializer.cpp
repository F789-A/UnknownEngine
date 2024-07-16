#include "AppInitializer.h"
#include "WindowApp.h"
#include "GameLoop.h"
#include "Audio/AlcDevice.h"
#include "Audio/AlcBuffer.h"
#include "Audio/AlSource.h"
#include "Assets/SoundImporter.h"

void RunApp()
{
	AlcDevice alc;
	WindowApp::GetInstance().CreateWindow();
	GameLoop::GetInstance().RunLoop();
	WindowApp::GetInstance().TerminateApp();
}