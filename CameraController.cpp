#include "CameraController.h"

CameraController::CameraController(float mouseSensitivity, float speed):
    MouseSensitivity(mouseSensitivity),
    Speed(speed)
{}

void CameraController::Update()
{
    Transform& transf = GetLinkedEntity().GetComponent<Transform>();
	Camera& camera = GetLinkedEntity().GetComponent<Camera>();
    
    glm::vec3 movement(0, 0, 0);
    if (Input::GetInstance().GetButton("Front", GLFW_PRESS) == true)
    {
        movement += transf.Front();
    }
    if (Input::GetInstance().GetButton("Backward", GLFW_PRESS) == true)
    {
        movement -= transf.Front();
    }
    if (Input::GetInstance().GetButton("Left", GLFW_PRESS) == true)
    {
        movement -= transf.Right();
    }
    if (Input::GetInstance().GetButton("Right", GLFW_PRESS) == true)
    {
        movement += transf.Right();
    }
    if (movement != glm::vec3(0, 0, 0))
    {
        float mult = AppTime::GetInstance().GetDeltaTime() * 1000;
        transf.Position += glm::normalize(movement) * Speed * mult;
    }

    float xoffset = -Input::GetInstance().MouseDeltaX() * MouseSensitivity;
    float yoffset = -Input::GetInstance().MouseDeltaY() * MouseSensitivity;

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