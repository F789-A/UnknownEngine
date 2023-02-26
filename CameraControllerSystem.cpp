#include "Systems.h"
#include "EcsSystem.h"
#include "Transform.h"
#include "Camera.h"
#include "Input.h"
#include "AppTime.h"
#include "CameraController.h"
#include <glm/gtx/vector_angle.hpp>

void CameraControllerSystem(ecs::EntityManager& em)
{
    for (auto l = em.GetComponents<MainCamera, CameraController, Transform>(); !l.end(); ++l)
    {
        auto [_m, controller, transf] = *l;

        glm::vec3 movement(0, 0, 0);
        if (Input::GetInstance().GetButton("Front", Input::PressMode::Repeat) == true)
        {
            movement += transf.Front();
        }
        if (Input::GetInstance().GetButton("Backward", Input::PressMode::Repeat) == true)
        {
            movement -= transf.Front();
        }
        if (Input::GetInstance().GetButton("Left", Input::PressMode::Repeat) == true)
        {
            movement -= transf.Right();
        }
        if (Input::GetInstance().GetButton("Right", Input::PressMode::Repeat) == true)
        {
            movement += transf.Right();
        }

        if (movement != glm::vec3(0, 0, 0))
        {
            float mult = AppTime::GetDeltaTime() * 1000;
            transf.Position += glm::normalize(movement) * controller.Speed * mult;
        }

        float xoffset = -Input::GetInstance().MouseDeltaX() * controller.MouseSensitivity;
        float yoffset = -Input::GetInstance().MouseDeltaY() * controller.MouseSensitivity;

        glm::vec3 normal = glm::cross(transf.Right(), transf.Front());
        float angle = glm::orientedAngle(normal, glm::vec3(0, 1, 0), transf.Right());
        if (angle < -1.5)
        {
            if (yoffset > 0)
            {
                yoffset = 0;
            }
        }
        if (angle > 1.5)
        {
            if (yoffset < 0)
            {
                yoffset = 0;
            }
        }
        transf.Rotate(glm::vec3(0, 1, 0), xoffset);
        transf.Rotate(transf.Right(), yoffset);
    }

}