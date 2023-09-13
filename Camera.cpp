#include "Camera.h"
#include "Transform.h"
#include "WindowApp.h"

glm::mat4 Camera::GetViewMatrix() const
{
    int a = ecs::DefEcs().entity.GetEntity<Camera>(*this);
    const auto& transf = ecs::DefEcs().entity.GetComponent<Transform>(a);
    return glm::lookAt(transf.Position, transf.Position + transf.Front(), glm::vec3(0, 1, 0));
}

glm::mat4 Camera::GetProjectionMatrix() const
{
    if (cameraType == CameraType::Perspective)
        return glm::perspective(FOV, (float)WindowApp::GetInstance().Width() / WindowApp::GetInstance().Height(), NearClip, FarClip);
    else
        return glm::ortho(0.0f, (float)WindowApp::GetInstance().Width(), 0.0f, (float)WindowApp::GetInstance().Height(), NearClip, FarClip);
}

int MainCamera::count = 0;