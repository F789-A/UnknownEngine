#pragma once
#include "EcsSystem.h"
#include <glm\vec3.hpp>

namespace Physics
{
    struct Gravity : public ecs::Component<Gravity>
    {
        float Acceleration = 9.8f;
        glm::vec3 Direction = { 0.0f, -1.0f, 0.0f };

        static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
        {
            auto& grav = em.GetComponent<Gravity>(ent);
        }
    };

}