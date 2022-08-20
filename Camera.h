#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "Component.h"
#include "Transform.h"
#include "ICamera.h"

class Camera: public ec::Component, public ICamera
{
public:
    Camera(GLfloat nearClip = 0.1f, GLfloat farClip = 100.0f, GLfloat zoom = glm::radians(45.0));
    ~Camera() override = default;

    GLfloat Zoom;
    GLfloat NearClip;
    GLfloat FarClip;
   
    glm::mat4 GetViewMatrix() const;
    float GetFOV() const;
};