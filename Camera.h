#pragma once
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Component.h"
#include "ILoopUpdate.h"
#include "Transform.h"
#include "EntityManager.h"
#include "ICamera.h"

class Camera: public Component, public ICamera
{
public:

    ~Camera() override = default;

    GLfloat Zoom;
    GLfloat NearClip;
    GLfloat FarClip;

    Camera(GLfloat NearClip = 0.1f, GLfloat FarClip = 100.0f);
    glm::mat4 GetViewMatrix();
    float GetFOV();

};