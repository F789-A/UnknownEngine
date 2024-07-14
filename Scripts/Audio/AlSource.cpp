#include "AlSource.h"

AlSource::AlSource(const AlcBuffer& buffer)
{
	alGenSources(1, &source);
	alSourcef(source, AL_PITCH, 1);
	alSourcef(source, AL_GAIN, 1.0f);
	alSource3f(source, AL_POSITION, 0, 0, 0);
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	alSourcei(source, AL_LOOPING, AL_FALSE);
	alSourcei(source, AL_BUFFER, buffer.buffer);
}

AlSource::~AlSource()
{
	alDeleteSources(1, &source);
}

void AlSource::Play()
{
	alSourcePlay(source);
}