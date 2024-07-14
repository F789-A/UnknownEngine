#include "AlcBuffer.h"
#include <iostream>

AlcBuffer::AlcBuffer(const SoundData& soundData)
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

AlcBuffer::~AlcBuffer()
{
	alDeleteBuffers(1, &buffer);
}