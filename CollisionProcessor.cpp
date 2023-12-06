#include "Systems.h"

#include <glm/glm.hpp>
#include <optional>

#include "PhysicalBody.h"
#include "DynamicBvh2D.h"
#include "Transform.h"

#include "AppTime.h"

using namespace physics;

float cross2(const glm::vec2& a, glm::vec2& b)
{
    return glm::length(glm::cross(glm::vec3(a.x, a.y, 0), glm::vec3(b.x, b.y, 0)));
}

std::optional<Collision> GetCollision(const Collider& A, const Collider& B)
{
    return A.shape->GetCollision(*B.shape);
}

void CorrectCollision(RigidBody& A, Transform& trA, RigidBody& B, Transform& trB, const Collision& collision)
{
    glm::vec2 normal = collision.normal;
    float penetration = 0.0f;
    const float percent = 0.8f;
    const float slop = 0.01f;
    glm::vec2 correction = std::max(penetration - slop, 0.0f) / (A.invMass + B.invMass) * percent * normal;
    //trA.Position -= A.invMass * glm::vec3(correction, 0.0f);
    //trB.Position += B.invMass * glm::vec3(correction, 0.0f);
}

void ProcessFriction(RigidBody& A, RigidBody& B, const Collision& collision, float reaction);

void ProcessReaction(RigidBody& A, RigidBody& B, const Collision& collision)
{
    glm::vec2 vel = A.velocity - B.velocity;

    float velocityProj = glm::dot(vel, collision.normal);

    if (velocityProj < 0)
    {
        return;
    }

    float locElasticity = std::min(A.elasticity, B.elasticity);

    float j = -(1 + locElasticity) * velocityProj / (A.invMass + B.invMass);

    glm::vec2 impulse = j * collision.normal;

    A.velocity += impulse * A.invMass;
    B.velocity -= impulse * B.invMass;

    //A.torque += cross2(collision.pos - glm::vec2(0, 0), impulse);
    //B.torque += cross2(collision.pos - glm::vec2(0, 0), -impulse);

    ProcessFriction(A, B, collision, j);
}

void ProcessFriction(RigidBody& A, RigidBody& B, const Collision& collision, float reaction)
{
    glm::vec2 vel = B.velocity - A.velocity;
    glm::vec2 tangent = { collision.normal.y, -collision.normal.x };

    float velocityProj = glm::dot(vel, tangent);

    float locElasticity = std::min(A.elasticity, B.elasticity);

    float j = -(1 + locElasticity) * velocityProj / (A.invMass + B.invMass);

    float staticFriction = (A.staticFriction + B.staticFriction) / 2.0f;
    float dynamicFriction = (A.dynamicFriction + B.dynamicFriction) / 2.0f;

    glm::vec2 frictionImpulse;
    if (abs(j) < abs(reaction) * staticFriction)
    {
        frictionImpulse = j * tangent;
    }
    else
    {
        frictionImpulse = -reaction * tangent * dynamicFriction;
    }

    A.velocity += frictionImpulse * A.invMass;
    B.velocity -= frictionImpulse * B.invMass;
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

            Circle circleA{ colliderA.shape->Center() + glm::vec2(transformA.Position), colliderA.shape->Size() * transformA.Scale.x };
            Circle circleB{ colliderB.shape->Center() + glm::vec2(transformB.Position), colliderB.shape->Size() * transformB.Scale.x };

            //auto collision = GetCollision(colliderA, colliderB);
            auto collision = circleA.GetCollision(circleB);

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

        float dt = std::min(AppTime::GetDeltaTime(), 0.01f);

        // ��������������� ����� ������
        body.velocity += (body.invMass * body.forse) * dt;
        body.angularVelocity += body.torque * body.invMomentOfInertia * dt;

        body.forse = glm::vec2{0.0f, 0.0f};
        body.torque = 0.0f;

        transf.Position += glm::vec3(body.velocity, 0.0f) * dt;
        transf.Rotate({ 1.0f, 0.0f, 0.0f }, body.angularVelocity * dt);
    }
}