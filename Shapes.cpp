#include "Shapes.h"

#include <algorithm>

namespace Physics
{
    Square::Square(const glm::vec2& min, const glm::vec2& max) : min(min), max(max)
    {}

    bool Square::IntersectWith(const Point& shape) const
    {
        return shape.origin.x <= max.x && shape.origin.y <= max.y && shape.origin.x >= min.x && shape.origin.y >= min.y;
    }

    bool Square::IntersectWith(const Ray& shape) const
    {
        glm::vec2 arr[4] = { min, {max.x, min.y}, max, {min.x, max.y} };
        auto getCoeff = [&](const glm::vec2& origin, const glm::vec2& dir)
        {
            float k = dir.y / dir.x;
            float b = origin.y - k * origin.x;
            return std::make_pair(k, b);
        };
        auto coefShape = getCoeff(shape.origin, shape.direction);
        float k = coefShape.first;
        float b = coefShape.second;
        for (int i = 0; i < 4; ++i)
        {
            glm::vec2 origin = arr[i];
            glm::vec2 dir = arr[i % 4] - arr[i];
            auto coef = getCoeff(origin, dir);

            float x = (b - coef.second) / (coef.first - k); // ?
            float y = k * x - b;
            glm::vec2 point = { x, y };

            if (glm::dot(point - shape.origin, shape.direction) >= 0)
            {
                auto len = (point - origin).length();
                if (0 < len && len < dir.length())
                {

                }
            }
        }

        return false;
    }

    glm::vec2 Square::Center() const
    {
        return (min + max) / 2.0f;
    }

    Circle::Circle(glm::vec2 origin, float radius) : origin(origin), radius(radius)
    {}

    bool Circle::IntersectWith(const Point& shape) const
    {
        glm::vec2 dir = shape.origin - origin;
        return (dir.x * dir.x + dir.y * dir.y) <= radius * radius;
    }

    bool Circle::IntersectWith(const Ray& shape) const
    {
        float k = shape.direction.y / shape.direction.x;
        float b = shape.origin.y - k * shape.origin.x;

        float B = 1 / glm::vec2(k, -1).length();

        k *= B;
        b *= B;

        float len = (k * origin.x + B * origin.y + b);

        if (radius >= len)
        {
            glm::vec2 point = { origin.x - len * k, origin.y - len * B };
            if (len == radius)
            {

            }
            else
            {
                float len2 = std::sqrt(radius * radius - len * len);
                glm::vec2 point1 = point + len2 * glm::vec2(B, -k);
                glm::vec2 point2 = point + len2 * glm::vec2(-B, +k);
            }

        }

        return false;
    }

    std::tuple<bool, Collision, Collision> IsCollision(const Square& A, const Square& B)
    {
        glm::vec2 dir = B.Center() - A.Center();

        float halfAx = (A.max.x - A.min.x) / 2;
        float halfBx = (B.max.x - B.min.x) / 2;

        float overlapX = halfAx + halfBx - std::abs(dir.x);

        if (overlapX >= 0)
        {
            float halfAy = (A.max.y - A.min.y) / 2;
            float halfBy = (B.max.y - B.min.y) / 2;

            float overlapY = halfAy + halfBy - std::abs(dir.y);

            if (overlapY >= 0)
            {
                if (overlapX >= overlapY)
                {
                    glm::vec2 normal = glm::vec2{ 0.0f, 1.0f } *sgn(dir.y);

                    float contactX = (std::max(A.min.x, B.min.x) + std::min(A.max.x, B.max.x)) / 2;

                    Collision ToA = { {contactX, A.Center().y + halfAy * sgn(dir.y)}, normal, overlapY };
                    Collision ToB = { {contactX, B.Center().y + halfBy * sgn(-dir.y)}, -normal, overlapY };
                    return { true, ToA, ToB };
                }
                else
                {
                    glm::vec2 normal = glm::vec2{ 1.0f, 0.0f } *sgn(dir.x);

                    float contactY = (std::max(A.min.y, B.min.y) + std::min(A.max.y, B.max.y)) / 2;

                    Collision ToA = { {contactY, A.Center().x + halfAx * sgn(dir.x)}, normal, overlapX };
                    Collision ToB = { {contactY, B.Center().x + halfBx * sgn(-dir.x)}, -normal, overlapX };
                    return { true, ToA, ToB };
                }

            }
        }
        return { false, {}, {} };
    }

    std::tuple<bool, Collision, Collision> IsCollision(const Circle& A, const Circle& B)
    {
        glm::vec2 dir = B.origin - A.origin;
        float dist = dir.x * dir.x + dir.y * dir.y;

        if (dist > A.radius * A.radius + B.radius * B.radius)
        {
            return { false, {}, {} };
        }

        if (dist != 0)
        {
            dir = glm::normalize(dir);
            dist = std::sqrtf(dist);
            Collision ToA = { A.origin + dir * A.radius, dir, dist - B.radius };
            Collision ToB = { B.origin + dir * A.radius, -dir, dist - A.radius };
            return { true, ToA, ToB };
        }
        else
        {
            Collision ToA = { A.origin, {0.0f, 1.0f}, A.radius };
            Collision ToB = { B.origin, {0.0f, -1.0f}, B.radius };
            return { true, ToA, ToB };
        }
    }

    std::tuple<bool, Collision, Collision> IsCollision(const Square& A, const Circle& B)
    {
        glm::vec2 dir = B.origin - A.Center();
        glm::vec2 closest = dir;

        float halfAx = (A.max.x - A.min.x) / 2;
        float halfAy = (A.max.y - A.min.y) / 2;

        closest.x = std::clamp(-halfAx, halfAx, closest.x);
        closest.y = std::clamp(-halfAy, halfAy, closest.y);

        glm::vec2 r = A.Center() - closest;

        float radius = std::sqrtf(std::abs(glm::dot(r, r)));

        Circle newCircle{ A.Center(), radius };

        return IsCollision(newCircle, B);
    }

}