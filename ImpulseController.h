#pragma once
#include "EcsSystem.h"

struct ImpulseController : public ecs::Component<ImpulseController>
{
    static constexpr std::string_view ComponentName = "ImpulseController";

    glm::vec2 impulse;

    static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
    {
        auto& ic = em.GetComponent<ImpulseController>(ent);
    }
};