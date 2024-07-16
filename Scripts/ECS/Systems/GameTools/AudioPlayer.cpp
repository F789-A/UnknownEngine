#include "ECS\Systems\Systems.h"

#include "ECS\Components\AudioSource.h"

void Audio::AudioPlayer(ecs::EntityManager& em)
{
	for (auto l = em.GetComponents<AudioSource>(); !l.end(); ++l)
	{
		auto [source] = *l;

		if (!source.isPlaying)
		{
			source.alSource->Play();
			source.isPlaying = true;
		}
	}

}