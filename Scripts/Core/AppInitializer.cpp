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

	auto sound = ImportSoundData("Sound/untitled.wav");
	AlcBuffer buff(sound);
	AlSource source(buff);
	source.Play();

	WindowApp::GetInstance().CreateWindow();
	GameLoop::GetInstance().RunLoop();
	WindowApp::GetInstance().TerminateApp();
}