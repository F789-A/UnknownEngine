#pragma once
#include "EcsSystem.h"
#include "Bvh.h"

namespace Physics
{ 
    struct DynamicBvh2D : public ecs::Component<DynamicBvh2D>
    {
        static constexpr std::string_view ComponentName = "DynamicBvh2D";

        Bvh2D<int> bvh = Bvh2D<int>({{0.0f, 0.0f}, {1.0f, 1.0f }});

        static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
        {
            auto& bvhTree = em.GetComponent<DynamicBvh2D>(ent);

            bvhTree.bvh = Bvh2D<int>({ {-4.0f, -4.0f}, {4.0f, 4.0f } });

            Circle a({ 0, 0 }, 1);
            Circle b({ 2, 2 }, 1);
            Circle c({ -2, -2 }, 1);
            Circle d({ -2, -3 }, 1);
            Circle e({ -1, 3 }, 1.5);

            bvhTree.bvh.Insert(a, 0);
            bvhTree.bvh.Insert(b, 1);
            bvhTree.bvh.Insert(c, 2);
            bvhTree.bvh.Insert(d, 3);
            bvhTree.bvh.Insert(e, 4);

            for (auto k : bvhTree.bvh.ProcessRawCollision())
            {
                std::cout << k.first << k.second << std::endl;
            }
        }
    };
}