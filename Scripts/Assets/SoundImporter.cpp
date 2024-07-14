#include "SoundImporter.h"

#include "AudioFile.h"

SoundData ImportSoundData(const std::string& path)
{
	SoundData soundData;

	AudioFile<double> audioFile;
	audioFile.load(path);

	soundData.sampleRate = audioFile.getSampleRate();
	soundData.bitsPerSample = audioFile.getBitDepth();
	soundData.numberOfChannels = audioFile.getNumChannels();

	for (int i = 0; i < soundData.numberOfChannels; ++i)
	{
		int numSamples = audioFile.getNumSamplesPerChannel();
		for (int j = 0; j < numSamples; j++)
		{
			soundData.data.push_back(audioFile.samples[i][j]);
		}
	}

	return soundData;
}