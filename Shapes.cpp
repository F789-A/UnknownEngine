#include "Shapes.h"

#include <glm/gtx/norm.hpp>
#include <algorithm>
#include <functional>

#include "DynamicDispatching.h"

namespace physics
{
    Line::Line(const glm::vec2& origin, const glm::vec2& direction)
    {
        if (direction.y == 0.0f)
        {
            A = 0.0f;
            B = 1.0f;
            C = -origin.y;
            return;
        }
        if (direction.x == 0.0f)
        {
            A = 1.0f;
            B = 0.0f;
            C = -origin.x;
            return;
        }
        float k = direction.y / direction.x;
        float normalized = 1.0f / glm::vec2(k, -1).length();

        A = -normalized;
        B = k * normalized;
        C = (origin.y - k * origin.x) * normalized;
    }

    std::optional<glm::vec2> Line::IntersectWith(const Line& other) const
    {
        if (std::abs(A) == std::abs(other.A) && std::abs(B) == std::abs(other.B))
        {
            return std::nullopt;
        }

        float k = other.A / A;
        float c = other.C + k * C;
        k = other.B - k * B;

        float y = -c / k;
        float x = (C - B * y) / A;
        return { {x, y} };
    }

    std::optional<glm::vec2> Interval::IntersectWith(const Ray& shape) const
    {
        auto rayLine = Line(shape.origin, shape.direction);
        auto intervalLine = Line(start, end - start);

        auto collision = intervalLine.IntersectWith(rayLine);

        if (!collision)
        {
            return std::nullopt;
        }

        glm::vec2 dir1 = start - *collision;
        glm::vec2 dir2 = end - *collision;
        glm::vec2 dir3 = *collision - shape.origin;

        if (glm::dot(dir1, dir2) < 0 && glm::dot(shape.direction, dir3) > 0)
        {
            return collision;
        }
        return std::nullopt;
    }

    Square::Square(const glm::vec2& min, const glm::vec2& max) : min(min), max(max)
    {}

    glm::vec2 Square::Center() const
    {
        return (min + max) / 2.0f;
    }

    float Square::Size() const
    {
        return glm::length(max - min) / 2;
    }

    bool Square::IntersectWith(const Point& shape) const
    {
        return shape.origin.x <= max.x && shape.origin.y <= max.y && shape.origin.x >= min.x && shape.origin.y >= min.y;
    }

    bool Square::IntersectWith(const Ray& ray) const
    {
        glm::vec2 corners[4] = { min, {max.x, min.y}, max, {min.x, max.y} };

        for (int i = 0; i < 4; ++i)
        {
            auto interval = Interval(corners[i], corners[(i + 1) % 4]);
            if (interval.IntersectWith(ray))
            {
                return true;
            }
        }

        return false;
    }

    Circle::Circle(glm::vec2 origin, float radius) : origin(origin), radius(radius)
    {}

    glm::vec2 Circle::Center() const
    {
        return origin;
    }

    float Circle::Size() const
    {
        return radius;
    }

    bool Circle::IntersectWith(const Point& shape) const
    {
        return glm::length2(shape.origin - origin) <= std::powf(radius, 2.0f);
    }

    bool Circle::IntersectWith(const Ray& shape) const
    {
        auto line = Line(shape.origin, shape.direction);

        float len = (line.A * origin.x + line.B * origin.y + line.C);

        if (radius >= len)
        {
            glm::vec2 point = { origin.x - len * line.A, origin.y - len * line.B };
            if (len != radius)
            {
                float len2 = std::sqrt(radius * radius - len * len);
                glm::vec2 point1 = point + len2 * glm::vec2(line.B, -line.A);
                glm::vec2 point2 = point + len2 * glm::vec2(-line.B, line.A);

                if (glm::dot(point1 - shape.origin, shape.direction) > 0)
                    return true;
                if (glm::dot(point2 - shape.origin, shape.direction) > 0)
                    return true;
            }
            else
            {
                if (glm::dot(point - shape.origin, shape.direction) > 0)
                    return true;
            }
        }
        return false;
    }

    std::optional<std::pair<Collision, Collision>> IsCollision(const Square& A, const Square& B)
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
                    return { {ToA, ToB} };
                }
                else
                {
                    glm::vec2 normal = glm::vec2{ 1.0f, 0.0f } *sgn(dir.x);

                    float contactY = (std::max(A.min.y, B.min.y) + std::min(A.max.y, B.max.y)) / 2;

                    Collision ToA = { {contactY, A.Center().x + halfAx * sgn(dir.x)}, normal, overlapX };
                    Collision ToB = { {contactY, B.Center().x + halfBx * sgn(-dir.x)}, -normal, overlapX };
                    return { {ToA, ToB} };
                }

            }
        }
        return std::nullopt;
    }
    std::optional<std::pair<Collision, Collision>> IsCollision(const Circle& A, const Circle& B)
    {
        glm::vec2 dir = B.origin - A.origin;
        float dist = dir.x * dir.x + dir.y * dir.y;

        if (dist > A.radius * A.radius + B.radius * B.radius)
        {
            return std::nullopt;
        }

        if (dist != 0)
        {
            dir = glm::normalize(dir);
            dist = std::sqrtf(dist);
            Collision ToA = { A.origin + dir * A.radius, dir, dist - B.radius };
            Collision ToB = { B.origin + dir * A.radius, -dir, dist - A.radius };
            return { { ToA, ToB } };
        }
        else
        {
            Collision ToA = { A.origin, {0.0f, 1.0f}, A.radius };
            Collision ToB = { B.origin, {0.0f, -1.0f}, B.radius };
            return { { ToA, ToB } };
        }
    }
    std::optional<std::pair<Collision, Collision>> IsCollision(const Square& A, const Circle& B)
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
    std::optional<std::pair<Collision, Collision>> IsCollision(const Circle& A, const Square& B)
    {
        auto res = IsCollision(B, A);
        if (res)
        {
            return { {(*res).second, (*res).first} };
        }
        return res;
    }

    std::optional<Collision> Shape::GetCollision(const Shape& shape) const
    {
        std::function<std::optional<std::pair<Collision, Collision>>(const Square&, const Square&)> func1 =
            static_cast<std::optional<std::pair<Collision, Collision>>(*)(const Square&, const Square&)>(IsCollision);
        std::function<std::optional<std::pair<Collision, Collision>>(const Square&, const Circle&)> func2 =
            static_cast<std::optional<std::pair<Collision, Collision>>(*)(const Square&, const Circle&)>(IsCollision);
        std::function<std::optional<std::pair<Collision, Collision>>(const Circle&, const Circle&)> func3 =
            static_cast<std::optional<std::pair<Collision, Collision>>(*)(const Circle&, const Circle&)>(IsCollision);
        std::function<std::optional<std::pair<Collision, Collision>>(const Circle&, const Square&)> func4 =
            static_cast<std::optional<std::pair<Collision, Collision>>(*)(const Circle&, const Square&)>(IsCollision);

        auto result = DynamicDispatch(*this, shape, func1, func2, func3, func4);

        if (!result)
        {
            return std::nullopt;
        }

        return (*result).first;
    }
}