#include "Systems.h"
#include "Gravity.h"
#include "PhysicalBody.h"

void physics::GravityController(ecs::EntityManager& em)
{
    for (auto g = em.GetComponents<GlobalGravity>(); !g.end(); ++g)
    {
        auto [globalGravity] = *g;

        for (auto l = em.GetComponents<Gravity, RigidBody>(); !l.end(); ++l)
        {
            auto [gravity, rigidbody] = *l;

            rigidbody.forse += glm::vec2(globalGravity.Direction.x, gravity.Direction.y) * gravity.Acceleration;
        }
    }
}