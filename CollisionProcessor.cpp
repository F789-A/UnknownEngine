#include "Systems.h"
#include <glm/glm.hpp>
#include "PhysicalBody.h"
#include "DynamicBvh2D.h"

using namespace Physics;

void physics::ProcessCollision(ecs::EntityManager& em)
{
    for (auto l = em.GetComponents<DynamicBvh2D>(); !l.end(); ++l)
    {
        auto [bvh] = *l;
        //bvh.findAllCollision();
    }
}

void physics::ProcessFriction(ecs::EntityManager& em)
{
    //for (auto l = em.GetComponents<>(); !l.end(); ++l)
    {
        
    }
}

void physics::ProcessMovement(ecs::EntityManager& em)
{
    for (auto l = em.GetComponents<RigidBody>(); !l.end(); ++l)
    {
        auto [body] = *l;

        float dt = 0.01f;

        // Симплектический метод Эйлера
        body.velocity += (body.invMass * body.forse) * dt;
        glm::vec2 transform; 
        transform += body.velocity * dt;
        body.forse = glm::vec2{0.0f, 0.0f};

        body.angularVelocity += body.torque * body.invMomentOfInertia * dt;

        //position += velocity * dt
        //orient += angularVelocity * dt

        body.torque = 0;
    }
}

void ResolceConflict(RigidBody& A, RigidBody& B)
{
    glm::vec2 normal;

    glm::vec2 vel = B.velocity - A.velocity;

    float velocityProj = glm::dot(vel, normal);

    if (velocityProj > 0)
    {
        return;
    }

    float locElasticity = std::min(A.elasticity, B.elasticity);

    float j = (-(1 + locElasticity) * velocityProj) / (A.invMass + B.invMass);

    A.forse += j * normal;
    B.forse += j * normal;
}

void CorrectCollision(RigidBody& A, RigidBody& B)
{
    glm::vec2 normal;
    float penetration = 0.0f;
    const float percent = 0.8f;
    const float slop = 0.01f;
    glm::vec2 correction = std::max(penetration - slop, 0.0f) / (A.invMass + B.invMass) * percent * normal;
    //other.position -= other.inv_mass * correction;
    //other.position += other.inv_mass * correction;
}

void ResolceFriction(RigidBody& A, RigidBody& B)
{
    glm::vec2 normal;

    glm::vec2 vel = B.velocity - A.velocity;

    glm::vec2 tangent = glm::normalize(vel - glm::dot(vel, normal) * normal);
   
    float velocityProj = glm::dot(vel, tangent);

    if (velocityProj > 0)
    {
        return;
    }

    float locElasticity = std::min(A.elasticity, B.elasticity);

    float j = (-(1 + locElasticity) * velocityProj) / (A.invMass + B.invMass);

    float m = (A.staticFriction + B.staticFriction) / 2.0f;

    float N = 0; // TODO
    glm::vec2 frictionImpulse;
    if (abs(j) < N * m) 
    {
        frictionImpulse = j * tangent;
    }
    else
    {
        float dynamicFriction = (A.dynamicFriction + B.dynamicFriction)/2.0f;
        frictionImpulse = -N * tangent * dynamicFriction;
    }

    A.forse += frictionImpulse;
    B.forse += frictionImpulse;
}