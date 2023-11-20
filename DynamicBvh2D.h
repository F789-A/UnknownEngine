#pragma once
#include "EcsSystem.h"
#include "Bvh.h"

namespace physics
{ 
    struct DynamicBvh2D : public ecs::Component<DynamicBvh2D>
    {
        static constexpr std::string_view ComponentName = "DynamicBvh2D";

        Bvh2D<int> bvh = Bvh2D<int>({{0.0f, 0.0f}, {1.0f, 1.0f }});

        static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
        {
            auto& bvhTree = em.GetComponent<DynamicBvh2D>(ent);
        }
    };
}