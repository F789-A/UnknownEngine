#pragma once
#include <string>
#include "ECS\EcsSystem.h"

#include "Audio/AlcDevice.h"
#include "Audio/AlcBuffer.h"
#include "Audio/AlSource.h"
#include "Assets/SoundImporter.h"

struct AudioSource : public ecs::Component<AudioSource>
{
    static constexpr std::string_view ComponentName = "AudioSource";

    std::unique_ptr<AlcBuffer> alBuffer;
    std::unique_ptr<AlSource> alSource;

    bool isPlaying = false;

    static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
    {
        auto& audioSource = em.GetComponent<AudioSource>(ent);

        auto sound = ImportSoundData(res.at("Sound"));

        audioSource.alBuffer = std::make_unique<AlcBuffer>(sound);
        audioSource.alSource = std::make_unique<AlSource>(*audioSource.alBuffer);

        audioSource.alSource->Play();
    }
};