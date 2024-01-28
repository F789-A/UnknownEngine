#include <Utils\SimpleMath.h>
#include <glm\glm.hpp>

float cross2(const glm::vec2& a, glm::vec2& b)
{
    return glm::cross(glm::vec3(a.x, a.y, 0), glm::vec3(b.x, b.y, 0)).z;
}