#include "Camera.h"
#include "Transform.h"

glm::mat4 Camera::GetViewMatrix() const
{
    int a = ECS::DefEcs_().entity.GetEntity<Camera>(*this);
    const auto& transf = ECS::DefEcs_().entity.GetComponent<Transform>(a);
    return glm::lookAt(transf.Position, transf.Position + transf.Front(), glm::vec3(0, 1, 0));
}

int MainCamera::count = 0;