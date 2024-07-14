#pragma once
#include <OpenALSoftdbg\al.h>
#include <OpenALSoftdbg\alc.h>

#include "Assets\SoundData.h"

class AlcBuffer
{
public:
	ALuint buffer;

	AlcBuffer(const SoundData& soundData);
	~AlcBuffer();
};