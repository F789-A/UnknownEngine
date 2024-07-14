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

class AlcBuffer
{
public:

	ALuint buffer;

	AlcBuffer(const SoundData& soundData)
	{
		alGenBuffers(1, &buffer);

		ALenum format;
		if (soundData.numberOfChannels == 1 && soundData.bitsPerSample == 8)
			format = AL_FORMAT_MONO8;
		else if (soundData.numberOfChannels == 1 && soundData.bitsPerSample == 16)
			format = AL_FORMAT_MONO16;
		else if (soundData.numberOfChannels == 2 && soundData.bitsPerSample == 8)
			format = AL_FORMAT_STEREO8;
		else if (soundData.numberOfChannels == 2 && soundData.bitsPerSample == 16)
			format = AL_FORMAT_STEREO16;


		alBufferData(buffer, format, soundData.data.data(), soundData.data.size(), soundData.sampleRate);
	}

	~AlcBuffer()
	{
		alDeleteBuffers(1, &buffer);
	}

};

class AlSource
{
	ALuint source;

public:

	AlSource(const AlcBuffer& buffer)
	{
		alGenSources(1, &source);
		alSourcef(source, AL_PITCH, 1);
		alSourcef(source, AL_GAIN, 1.0f);
		alSource3f(source, AL_POSITION, 0, 0, 0);
		alSource3f(source, AL_VELOCITY, 0, 0, 0);
		alSourcei(source, AL_LOOPING, AL_FALSE);
		alSourcei(source, AL_BUFFER, buffer.buffer);
	}

	~AlSource()
	{
		alDeleteSources(1, & source);
	}

	void Play()
	{
		alSourcePlay(source);
	}
};