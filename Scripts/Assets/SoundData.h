#pragma once
#include <vector>

class SoundData
{
public:
	int numberOfChannels;
	int bitsPerSample;
	int sampleRate;
	std::vector<double> data;
};
