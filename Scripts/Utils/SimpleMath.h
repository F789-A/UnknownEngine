#pragma once
#include <glm\glm.hpp>

template <typename T>
T sgn(const T& val)
{
    return (T(0) < val) - (val < T(0));
}

float cross2(const glm::vec2& a, const glm::vec2& b);

glm::vec2 normalExternalCCV(const glm::vec2& a);

glm::vec2 orthogonalize(const glm::vec2& relatively, const glm::vec2& b);