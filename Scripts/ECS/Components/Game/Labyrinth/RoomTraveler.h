#pragma once

#include "ECS\EcsSystem.h"

struct RoomTraveler : public ecs::Component<RoomTraveler>
{
    static constexpr std::string_view ComponentName = "RoomTraveler";

    int PreviousRoom = -1;
    int CurrentRoom = 0;

    static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
    {
        auto& rt = em.GetComponent<RoomTraveler>(ent);
    }
};