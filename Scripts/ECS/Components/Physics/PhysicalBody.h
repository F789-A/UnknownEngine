#pragma once
#include <memory>
#include <utility>
#include <bitset>
#include <string>
#include <glm/glm.hpp>

#include "ECS\EcsSystem.h"

#include "Utils\SimpleTextProcessor.h"
#include "Physics\Shapes.h"

namespace physics
{
    struct RigidBody : public ecs::Component<RigidBody>
    {
        static constexpr std::string_view ComponentName = "RigidBody";

        glm::vec2 velocity;
        glm::vec2 forse;
        float angularVelocity;
        float torque;

        float invMass;
        float elasticity;
        float staticFriction;
        float dynamicFriction;
        float invMomentOfInertia;

        std::bitset<10> layer;

        static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
        {
            auto& body = em.GetComponent<RigidBody>(ent);
            body.invMass = 1.0f / std::stof(res["Mass"]);
            body.elasticity = std::stof(res["Elasticity"]);
            body.staticFriction = std::stof(res["StaticFriction"]);
            body.dynamicFriction = std::stof(res["DynamicFriction"]);
            body.invMomentOfInertia = 1.0f / std::stof(res["MomentOfInertia"]);

            if (body.staticFriction < body.dynamicFriction)
            {
                throw "not correct";
            }
        }
    };

    struct Gravity : public ecs::Component<Gravity>
    {
        static constexpr std::string_view ComponentName = "Gravity";

        static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
        {
        }
    };
}