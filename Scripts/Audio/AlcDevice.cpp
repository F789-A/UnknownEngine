#include "AlcDevice.h"

AlcDevice::AlcDevice()
{
	openALDevice = alcOpenDevice(nullptr);
	if (!openALDevice)
	{
		std::cout << "openAl failed";
	}

	openALContext = alcCreateContext(openALDevice, nullptr);
	ALCboolean contextMadeCurrent = alcMakeContextCurrent(openALContext);
	if (!contextMadeCurrent)
	{
		std::cout << "openAl failed";
	}
}

AlcDevice::~AlcDevice()
{
	ALCboolean contextMadeCurrent = alcMakeContextCurrent(nullptr);
	alcDestroyContext(openALContext);
	ALCboolean closed = alcCloseDevice(openALDevice);
}