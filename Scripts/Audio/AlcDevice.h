#pragma once
#include <iostream>

#include <OpenALSoftdbg\al.h>
#include <OpenALSoftdbg\alc.h>

#include "Assets\SoundData.h"

class AlcDevice
{
private:
	ALCdevice* openALDevice;
	ALCcontext* openALContext;

public:
	AlcDevice();
	~AlcDevice();
};