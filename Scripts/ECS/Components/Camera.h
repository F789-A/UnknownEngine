#pragma once
#include <string>
#include <glm/glm.hpp>
#include "ECS\EcsSystem.h"

enum class CameraType 
{
    Orthographic,
    Perspective
};

struct Camera: public ecs::Component<Camera>
{
    static constexpr std::string_view ComponentName = "Camera";

    float FOV = glm::radians(45.0f);
    float NearClip = 0.1f;
    float FarClip = 100.0f;
    float Size = 10.0f;
    CameraType cameraType = CameraType::Perspective;

    static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
    {
        Camera& cam = em.GetComponent<Camera>(ent);
        if (res.contains("FOV"))
        {
            cam.FOV = glm::radians(std::stof(res["FOV"]));
            cam.cameraType = CameraType::Perspective;
        }
        else
        {
            cam.Size = std::stof(res["Size"]);
            cam.cameraType = CameraType::Orthographic;
        }
        cam.NearClip = std::stof(res["NearClip"]);
        cam.FarClip = std::stof(res["FarClip"]);
    }

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;
};

struct MainCamera : public ecs::Component<MainCamera>
{
    static constexpr std::string_view ComponentName = "MainCamera";

    static int count;
    MainCamera()
    {
        count++;
        if (count > 1)
        {
            throw "две камеры";
        }
    }
    static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
    {}
    ~MainCamera()
    {
        count--;
    }

};