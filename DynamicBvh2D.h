#pragma once
#include "EcsSystem.h"
#include "Bvh.h"

namespace Physics
{ 
    struct DynamicBvh2D : public ecs::Component<DynamicBvh2D>
    {
        Bvh2D bvh = Bvh2D({{0.0f, 0.0f}, {1.0f, 1.0f }});

        static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
        {
            auto& body = em.GetComponent<DynamicBvh2D>(ent);
        }
    };
}