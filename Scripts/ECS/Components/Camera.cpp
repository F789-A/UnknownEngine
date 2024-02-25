#include "Camera.h"
#include "Transform.h"
#include "Graphics\GraphicCore.h"

glm::mat4 Camera::GetViewMatrix() const
{
    int a = ecs::EcsSystem::GetInstance().entity.GetEntity<Camera>(*this);
    const auto& transf = ecs::EcsSystem::GetInstance().entity.GetComponent<Transform>(a);
    return glm::lookAt(transf.Position, transf.Position + transf.Front(), glm::vec3(0, 1, 0));
}

glm::mat4 Camera::GetProjectionMatrix() const
{
    float aspect = GraphicCore::GetInstance().GetAspect();
    if (cameraType == CameraType::Perspective) {
        return glm::perspective(FOV, aspect, NearClip, FarClip);
    }
    else 
    {
        float h = Size / aspect / 2.0f;
        float w = Size / 2.0f;
        return glm::ortho(-w, w, -h, h, NearClip, FarClip);
    }
        
}

int MainCamera::count = 0;