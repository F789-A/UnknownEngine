#include "Camera.h"
#include <iostream>

Camera::Camera(GLfloat NearClip, GLfloat FarClip) :
    Zoom(45.0f),
    NearClip(NearClip),
    FarClip(FarClip),
    transf(EntityManager::Manager().GetComponent<Transform>(2))
{
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(transf.Position, transf.Position + transf.Front(), glm::vec3(0, 1, 0));
}

float Camera::GetFOV()
{
    return glm::radians(this->Zoom);
}