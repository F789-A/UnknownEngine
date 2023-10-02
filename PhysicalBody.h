#pragma once
#include "EcsSystem.h"
#include <glm/glm.hpp>
#include <utility>
#include <bitset>

namespace Physics
{
    struct RigidBody : public ecs::Component<RigidBody>
    {
        glm::vec2 velocity;
        glm::vec2 forse;
        float invMass = 1;
        float elasticity;

        float invMomentOfInertia;
        float angularVelocity;
        float torque;
        float staticFriction;
        float dynamicFriction;

        std::bitset<10> layer;

        static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
        {
            auto& body = em.GetComponent<RigidBody>(ent);
        }
    };
}