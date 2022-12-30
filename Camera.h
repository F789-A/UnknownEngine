#pragma once
#include "ecs_EntityManager.h"
#include <glm/glm.hpp>


class Camera: public ECS::Component<Camera>
{
public:
    float FOV = glm::radians(45.0);
    float NearClip = 0.1f;
    float FarClip = 100.0f;

    glm::mat4 GetViewMatrix() const;
};

class MainCamera : public ECS::Component<MainCamera>
{
public:
    static int count;
    MainCamera()
    {
        count++;
        if (count > 1)
        {
            throw "две камеры";
        }
    }

    ~MainCamera()
    {
        count--;
    }

};