#include "ECS\Systems\Systems.h"

#include <optional>
#include <glm/glm.hpp>

#include "ECS\Components\Physics\PhysicalBody.h"
#include "ECS\Components\Physics\Collider.h"
#include "ECS\Components\Physics\DynamicBvh2D.h"
#include "ECS\Components\Transform.h"

#include "Core\AppTime.h"

#include "Utils\SimpleMath.h"

using namespace physics;

void CorrectCollision(RigidBody& A, Transform& trA, RigidBody& B, Transform& trB, const Collision& collision)
{
    glm::vec2 normal = collision.normal;
    float penetration = collision.penetration;
    constexpr float percent = 0.8f;
    constexpr float slop = 0.01f;
    glm::vec2 correction = std::max(penetration - slop, 0.0f) / (A.invMass + B.invMass) * percent * normal;
    trA.Position -= A.invMass * glm::vec3(correction, 0.0f);
    trB.Position += B.invMass * glm::vec3(correction, 0.0f);
}

void ProcessFriction(RigidBody& A, RigidBody& B, const glm::vec2& centerA, const glm::vec2& centerB, const Collision& collision, float reaction);

void ProcessReaction(RigidBody& A, RigidBody& B, const glm::vec2& centerA, const glm::vec2& centerB, const Collision& collision)
{
    glm::vec2 vel = A.velocity - B.velocity;

    float velocityProj = glm::dot(vel, collision.normal);

    if (velocityProj < 0)
    {
        return;
    }

    float locElasticity = std::min(A.elasticity, B.elasticity);

    float impulseAmount = -(1 + locElasticity) * velocityProj / (A.invMass + B.invMass);

    glm::vec2 impulse = impulseAmount * collision.normal;

    A.velocity += impulse * A.invMass;
    B.velocity -= impulse * B.invMass;

    A.angularVelocity += -cross2(centerA - collision.pos, impulse) * A.invMomentOfInertia;
    B.angularVelocity -= -cross2(centerB - collision.pos, impulse) * B.invMomentOfInertia;

    ProcessFriction(A, B, centerA, centerB, collision, impulseAmount);
}

void ProcessFriction(RigidBody& A, RigidBody& B, const glm::vec2& centerA, const glm::vec2& centerB, const Collision& collision, float reaction)
{ 
    glm::vec2 vel = A.velocity - B.velocity;
    glm::vec2 tangent = { collision.normal.y, -collision.normal.x };

    float velocityProj = glm::dot(vel, tangent);

    float locElasticity = std::min(A.elasticity, B.elasticity);
    float impulseAmount = -(1 + locElasticity) * velocityProj / (A.invMass + B.invMass);

    float staticFriction = (A.staticFriction + B.staticFriction) / 2.0f;

    glm::vec2 frictionImpulse;
    if (std::abs(impulseAmount) < std::abs(reaction) * staticFriction)
    {
        frictionImpulse = impulseAmount * tangent;
    }
    else
    {
        float dynamicFriction = (A.dynamicFriction + B.dynamicFriction) / 2.0f;
        frictionImpulse = -sgn(velocityProj) * std::abs(reaction) * tangent * dynamicFriction;
    }

    A.velocity += frictionImpulse * A.invMass;
    B.velocity -= frictionImpulse * B.invMass;

    A.angularVelocity += -cross2(centerA - collision.pos, frictionImpulse) * A.invMomentOfInertia;
    B.angularVelocity -= -cross2(centerB - collision.pos, frictionImpulse) * B.invMomentOfInertia;
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
            Circle circle{ collider.shape->Center() + glm::vec2(transform.Position), collider.shape->Size() * transform.Scale.x };

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

            auto& transformA = em.GetComponent<Transform>(l.first);
            auto& transformB = em.GetComponent<Transform>(l.second);

            std::unique_ptr<Shape> shapeA = ApplyTransformToShape(*colliderA.shape, glm::vec2(transformA.Position), 
                glm::vec2(transformA.Scale));
            std::unique_ptr<Shape> shapeB = ApplyTransformToShape(*colliderB.shape, glm::vec2(transformB.Position),
                glm::vec2(transformB.Scale));

            auto collision = shapeA->GetCollision(*shapeB);

            if (collision)
            {
                auto& rigidBodyA = em.GetComponent<RigidBody>(l.first);
                auto& rigidBodyB = em.GetComponent<RigidBody>(l.second);

                auto& transformA = em.GetComponent<Transform>(l.first);
                auto& transformB = em.GetComponent<Transform>(l.second);

                CorrectCollision(rigidBodyA, transformA, rigidBodyB, transformB, *collision);

                ProcessReaction(rigidBodyA, rigidBodyB, shapeA->Center(), shapeB->Center(), *collision);
            }
        }
    }
}

void physics::ProcessMovement(ecs::EntityManager& em)
{
    for (auto l = em.GetComponents<RigidBody, Transform>(); !l.end(); ++l)
    {
        auto [body, transf] = *l;

        float dt = std::min(AppTime::GetDeltaTime(), 0.01f);
        // Симплектический метод Эйлера
        body.velocity += (body.invMass * body.forse) * dt;
        body.angularVelocity += body.torque * body.invMomentOfInertia * dt;

        body.forse = glm::vec2{0.0f, 0.0f};
        body.torque = 0.0f;

        transf.Position += glm::vec3(body.velocity, 0.0f) * dt;
        transf.Rotate({ 0.0f, 0.0f, 1.0f }, body.angularVelocity * dt);
    }
}