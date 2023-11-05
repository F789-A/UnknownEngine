#include "Systems.h"

#include <glm/glm.hpp>
#include <optional>

#include "PhysicalBody.h"
#include "DynamicBvh2D.h"
#include "Transform.h"

using namespace Physics;

std::optional<Collision> GetCollision(const Collider& A, const Collider& B)
{
    return A.shape->GetCollision(*B.shape);
}

void CorrectCollision(RigidBody& A, Transform& trA, RigidBody& B, Transform& trB, const Collision& collision)
{
    glm::vec2 normal;
    float penetration = 0.0f;
    const float percent = 0.8f;
    const float slop = 0.01f;
    glm::vec2 correction = std::max(penetration - slop, 0.0f) / (A.invMass + B.invMass) * percent * normal;
    trA.Position -= A.invMass * glm::vec3(correction, 0.0f);
    trB.Position += B.invMass * glm::vec3(correction, 0.0f);
}

void ProcessFriction(RigidBody& A, RigidBody& B, const Collision& collision, float reaction);

void ProcessReaction(RigidBody& A, RigidBody& B, const Collision& collision)
{
    glm::vec2 vel = B.velocity - A.velocity;

    float velocityProj = glm::dot(vel, collision.normal);

    if (velocityProj > 0)
    {
        return;
    }

    float locElasticity = std::min(A.elasticity, B.elasticity);

    float j = (-(1 + locElasticity) * velocityProj) / (A.invMass + B.invMass);

    A.forse += j * collision.normal;
    B.forse -= j * collision.normal;

    ProcessFriction(A, B, collision, j);
}

void ProcessFriction(RigidBody& A, RigidBody& B, const Collision& collision, float reaction)
{
    glm::vec2 vel = B.velocity - A.velocity;

    glm::vec2 tangent = glm::normalize(vel - glm::dot(vel, collision.normal) * collision.normal);

    float velocityProj = glm::dot(vel, tangent);

    if (velocityProj > 0)
    {
        return;
    }

    float locElasticity = std::min(A.elasticity, B.elasticity);

    float j = (-(1 + locElasticity) * velocityProj) / (A.invMass + B.invMass); //TODO something wrong

    float m = (A.staticFriction + B.staticFriction) / 2.0f;

    glm::vec2 frictionImpulse;
    if (abs(j) < reaction * m)
    {
        frictionImpulse = j * tangent;
    }
    else
    {
        float dynamicFriction = (A.dynamicFriction + B.dynamicFriction) / 2.0f;
        frictionImpulse = -reaction * tangent * dynamicFriction;
    }

    A.forse += frictionImpulse;
    B.forse += frictionImpulse;
}

void physics::BuildBvh(ecs::EntityManager& em)
{
    for (auto l = em.GetComponents<DynamicBvh2D>(); !l.end(); ++l)
    {
        auto [bvh] = *l;

        //not optimal: reset bvh and build again
        bvh.bvh = Bvh2D<int>({ {-100.0f, -100.0f}, {100.0f, 100.0f } });
        for (auto l = em.GetComponents<RigidBody, Collider, Transform>(); !l.end(); ++l)
        {
            auto [rigidbody, collider, transform] = *l;

            int ent = em.GetEntity(collider);
            Circle circle{ collider.shape->Center() + glm::vec2(transform.Position), collider.shape->Size() };

            bvh.bvh.Insert(circle, ent);

        }
    }
}

void physics::ProcessCollision(ecs::EntityManager& em)
{
    for (auto l = em.GetComponents<DynamicBvh2D>(); !l.end(); ++l)
    {
        auto [bvh] = *l;
        auto contactEntites = bvh.bvh.ProcessRawCollision();
        for (auto& l : contactEntites)
        {
            auto& colliderA = em.GetComponent<Collider>(l.first);
            auto& colliderB = em.GetComponent<Collider>(l.second);
            auto collision = GetCollision(colliderA, colliderB);
            if (collision)
            {
                auto& rigidBodyA = em.GetComponent<RigidBody>(l.first);
                auto& rigidBodyB = em.GetComponent<RigidBody>(l.second);

                auto& transformA = em.GetComponent<Transform>(l.first);
                auto& transformB = em.GetComponent<Transform>(l.second);

                CorrectCollision(rigidBodyA, transformA, rigidBodyB, transformB, *collision);

                ProcessReaction(rigidBodyA, rigidBodyB, *collision);
            }
        }
    }
}

void physics::ProcessMovement(ecs::EntityManager& em)
{
    for (auto l = em.GetComponents<RigidBody, Transform>(); !l.end(); ++l)
    {
        auto [body, transf] = *l;

        float dt = 0.01f;

        // Симплектический метод Эйлера
        body.velocity += (body.invMass * body.forse) * dt;
        body.angularVelocity += body.torque * body.invMomentOfInertia * dt;

        body.forse = glm::vec2{0.0f, 0.0f};
        body.torque = 0.0f;

        transf.Position += glm::vec3(body.velocity, 0.0f) * dt;
        transf.Rotate({ 0.0f, 0.0f, 1.0f }, body.angularVelocity * dt);
    }
}