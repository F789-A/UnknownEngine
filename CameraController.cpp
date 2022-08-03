#include "CameraController.h"


void CameraController::Update()
{
    Transform& transf = EntityManager::Manager().GetComponent<Transform>(LinkedEntity);
	Camera& camera = EntityManager::Manager().GetComponent<Camera>(LinkedEntity);
    if (Input::GetInstance().GetButton("Front", GLFW_PRESS) == true)
    {
        transf.Position += transf.Front() * Speed;
    }
    if (Input::GetInstance().GetButton("Backward", GLFW_PRESS) == true)
    {
        transf.Position -= transf.Front() * Speed;
    }
    if (Input::GetInstance().GetButton("Left", GLFW_PRESS) == true)
    {
        transf.Position -= transf.Right() * Speed;
    }
    if (Input::GetInstance().GetButton("Right", GLFW_PRESS) == true)
    {
        transf.Position += transf.Right() * Speed;
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

/*void CameraController::ProcessMouseScroll(GLfloat yoffset)
{
    if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
    {
        this->Zoom -= yoffset;
    }
    if (this->Zoom <= 1.0f)
        this->Zoom = 1.0f;
    if (this->Zoom >= 45.0f)
        this->Zoom = 45.0f;
}*/