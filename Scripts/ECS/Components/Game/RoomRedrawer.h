#pragma once
#include "ECS\EcsSystem.h"
#include <random>
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

    std::mt19937 random_generator;

    glm::vec3 RoomSize{ 10.0f, 7.5f, 6.0f };
    glm::vec3 CenterPos = {0.0f, 0.0f, 12.0f};
    float CenterScale = 0.5f;
    int BoxXCount = 12;
    int BoxYCount = 9;
    int BoxZCount = 5;

    static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
    {
        auto& rr = em.GetComponent<RoomVisual>(ent);

        unsigned int seed = 22124u;
        std::seed_seq seed_w({ seed });
        rr.random_generator = std::mt19937(seed_w);
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

    bool isActive = false;
    bool isDrawed = false;

    static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
    {
        auto& gc = em.GetComponent<GridController>(ent);
    }
};