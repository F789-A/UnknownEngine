#include "LegacyCamera.h"

LegacyCamera::LegacyCamera(glm::vec3 position, glm::vec3 up,
    GLfloat yaw, GLfloat pitch, GLfloat NearClip, GLfloat FarClip) :
    Front(glm::vec3(0.0f, 0.0f, -1.0f)),
    MovementSpeed(SPEED),
    MouseSensitivity(SENSITIVTY),
    Zoom(ZOOM),
    NearClip(NearClip),
    FarClip(FarClip)
{
    this->Position = position;
    this->WorldUp = up;
    this->Yaw = yaw;
    this->Pitch = pitch;

    lastMouseX = 0;
    lastMouseY = 0;

    this->updateCameraVectors();
}

glm::mat4 LegacyCamera::GetViewMatrix()
{
    return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
}

float LegacyCamera::GetFOV()
{
    return glm::radians(this->Zoom);
}

//INPUT
void LegacyCamera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
    GLfloat velocity = this->MovementSpeed * deltaTime;
    if (direction == FORWARD)
        this->Position += this->Front * velocity;
    if (direction == BACKWARD)
        this->Position -= this->Front * velocity;
    if (direction == LEFT)
        this->Position -= this->Right * velocity;
    if (direction == RIGHT)
        this->Position += this->Right * velocity;
}

void LegacyCamera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset)
{
    xoffset *= this->MouseSensitivity;
    yoffset *= this->MouseSensitivity;

    this->Yaw += (xoffset - lastMouseX);
    this->Pitch += lastMouseY - yoffset;

    if (this->Pitch > 89.0f)
    {
        this->Pitch = 89.0f;
    }
    if (this->Pitch < -89.0f)
    {
        this->Pitch = -89.0f;
    }

    lastMouseX = xoffset;
    lastMouseY = yoffset;

    this->updateCameraVectors();
}

void LegacyCamera::ProcessMouseScroll(GLfloat yoffset)
{
    if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
    {
        this->Zoom -= yoffset;
    }
    if (this->Zoom <= 1.0f)
        this->Zoom = 1.0f;
    if (this->Zoom >= 45.0f)
        this->Zoom = 45.0f;
}

void LegacyCamera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
    front.y = sin(glm::radians(this->Pitch));
    front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
    this->Front = glm::normalize(front);
    this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
    this->Up = glm::normalize(glm::cross(this->Right, this->Front));
}