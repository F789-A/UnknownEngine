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
    {
        float aspect = (float)WindowApp::GetInstance().Height() / WindowApp::GetInstance().Width();
        float h = Height * aspect / 2.0f; // TODO
        float w = Width / 2.0f;
        return glm::ortho(-w, w, -h, h, NearClip, FarClip);
    }
        
}

int MainCamera::count = 0;