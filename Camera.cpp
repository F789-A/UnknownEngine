#include "Camera.h"
#include <iostream>

Camera::Camera(GLfloat NearClip, GLfloat FarClip) :
    Zoom(45.0f),
    NearClip(NearClip),
    FarClip(FarClip)
{}

glm::mat4 Camera::GetViewMatrix()
{
    auto& transf = GetLinkedEntity().GetComponent<Transform>();
    return glm::lookAt(transf.Position, transf.Position + transf.Front(), glm::vec3(0, 1, 0));
}

float Camera::GetFOV()
{
    return glm::radians(this->Zoom);
}