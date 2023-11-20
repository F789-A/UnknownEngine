#pragma once
#include "EcsSystem.h"

#include <memory>
#include <utility>
#include <bitset>
#include <string>
#include <glm/glm.hpp>

#include "SimpleTextProcessor.h"
#include "Shapes.h"

namespace physics
{
    struct RigidBody : public ecs::Component<RigidBody>
    {
        static constexpr std::string_view ComponentName = "RigidBody";

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

    struct Gravity : public ecs::Component<Gravity>
    {
        static constexpr std::string_view ComponentName = "Gravity";

        float Acceleration = 9.8f;
        glm::vec3 Direction = { 0.0f, -1.0f, 0.0f };

        static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
        {
            auto& grav = em.GetComponent<Gravity>(ent);
        }
    };

    struct Collider : public ecs::Component<Collider>
    {
        static constexpr std::string_view ComponentName = "Collider";

        std::unique_ptr<Shape> shape;

        static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
        {
            auto& collider = em.GetComponent<Collider>(ent);
            if (res["shape"] == "Circle")
            {
                float radius = std::stof(res["rad"]);
                collider.shape = std::make_unique<Circle>(glm::vec2(0, 0), radius);
            }
            else if (res["shape"] == "Square")
            {
                float len = std::stof(res["len"]);
                collider.shape = std::make_unique<Square>(glm::vec2(-len, -len), glm::vec2(len, len));
            }
        }
    };
}