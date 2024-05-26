#include <Utils\SimpleMath.h>
#include <glm\glm.hpp>

float cross2(const glm::vec2& a, const glm::vec2& b)
{
    return glm::cross(glm::vec3(a.x, a.y, 0), glm::vec3(b.x, b.y, 0)).z;
}

glm::vec2 normalExternalCCV(const glm::vec2& a)
{
    return { a.y, -a.x };
}

glm::vec2 orthogonalize(const glm::vec2& relatively, const glm::vec2& b)
{
    return glm::cross(glm::cross(glm::vec3(relatively, 0.0f), glm::vec3(b, 0.0f)), glm::vec3(relatively, 0.0f));
}