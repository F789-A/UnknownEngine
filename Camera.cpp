#include "Camera.h"

Camera::Camera(GLfloat NearClip, GLfloat FarClip, GLfloat zoom) : 
    Zoom(zoom),
    NearClip(NearClip),
    FarClip(FarClip)
{}

glm::mat4 Camera::GetViewMatrix() const
{
    const auto& transf = GetLinkedEntity().GetComponent<Transform>();
    return glm::lookAt(transf.Position, transf.Position + transf.Front(), glm::vec3(0, 1, 0));
}

float Camera::GetFOV() const
{
    return Zoom;
}