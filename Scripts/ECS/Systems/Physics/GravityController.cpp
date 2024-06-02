#include "ECS\Systems\Systems.h"
#include "ECS\Components\Physics\Gravity.h"
#include "ECS\Components\Physics\PhysicalBody.h"

void physics::GravityController(ecs::EntityManager& em)
{
    for (auto g = em.GetComponents<GlobalGravity>(); !g.end(); ++g)
    {
        auto [globalGravity] = *g;

        for (auto l = em.GetComponents<Gravity, RigidBody>(); !l.end(); ++l)
        {
            auto [gravity, rigidbody] = *l;

            rigidbody.forse += glm::vec2(globalGravity.Direction.x, globalGravity.Direction.y) * globalGravity.Acceleration;
        }
    }
}