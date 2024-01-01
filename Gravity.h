#pragma once
#include "EcsSystem.h"
#include <glm\vec3.hpp>

#include "SimpleTextProcessor.h"

namespace physics
{
    struct GlobalGravity : public ecs::Component<GlobalGravity>
    {
        static constexpr std::string_view ComponentName = "GlobalGravity";

        float Acceleration;
        glm::vec3 Direction;

        static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
        {
            auto& grav = em.GetComponent<GlobalGravity>(ent);
            grav.Acceleration = std::stof(res["Acceleration"]);
            grav.Direction = TextTools::ReadVec3(res["Direction"]);
        }
    };
}