#pragma once
#include "AlcBuffer.h"

class AlSource
{
	ALuint source;
public:
	AlSource(const AlcBuffer& buffer);
	~AlSource();

	void Play();
};