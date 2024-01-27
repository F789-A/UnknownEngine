#pragma once
#include "ECS\EcsSystem.h"

struct RoomRedrawer : public ecs::Component<RoomRedrawer>
{
    static constexpr std::string_view ComponentName = "RoomRedrawer";

    bool NeedRedraw = true;
    std::vector<int> Entites;

    static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
    {
        auto& rr = em.GetComponent<RoomRedrawer>(ent);
    }
};