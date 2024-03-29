#pragma once
#include "ECS\EcsSystem.h"
#include <array>
#include <glm\glm.hpp>

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

struct RoomVisual : public ecs::Component<RoomVisual>
{
    static constexpr std::string_view ComponentName = "RoomVisual";

    glm::vec2 CenterPos = {0.0f, 0.5f};
    glm::vec2 CenterSize = {3.0f, 0.0f};
    int BoxXCount = 12;
    int BoxYCount = 9;
    int BoxZCount = 5;

    static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
    {
        auto& rr = em.GetComponent<RoomVisual>(ent);
    }
};

struct RoomTag : public ecs::Component<RoomTag>
{
    static constexpr std::string_view ComponentName = "RoomTag";

    static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
    {
        auto& rt = em.GetComponent<RoomTag>(ent);
    }
};

struct GridController : public ecs::Component<GridController>
{
    static constexpr std::string_view ComponentName = "GridController";

    std::array<glm::vec2, 4> points;
    bool isActive = false;
    bool isDrawed = false;

    static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
    {
        auto& gc = em.GetComponent<GridController>(ent);
    }
};