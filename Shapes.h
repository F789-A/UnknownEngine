#pragma once
#include <glm\glm.hpp>
#include <tuple>

template <typename T>
T sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}

namespace Physics
{ 
    class Shape {};

    struct Collision
    {
        glm::vec2 pos;
        glm::vec2 normal;
        float penetration;
    };

    struct Point
    {
        glm::vec2 origin;
    };

    struct Ray
    {
        glm::vec2 origin;
        glm::vec2 direction;
    };

    struct Square : public Shape
    {
        Square(const glm::vec2& min, const glm::vec2& max);

        glm::vec2 min;
        glm::vec2 max;

        glm::vec2 Center() const;

        bool IntersectWith(const Point& shape) const; // bad
        bool IntersectWith(const Ray& shape) const; // bad
    };

    struct Circle : public Shape
    {
        Circle(glm::vec2 origin, float radius);
        
        glm::vec2 origin;
        float radius;

        bool IntersectWith(const Point& shape) const; // bad
        bool IntersectWith(const Ray& shape) const; // bad
    };

    std::tuple<bool, Collision, Collision> IsCollision(const Square& A, const Square& B);
    std::tuple<bool, Collision, Collision> IsCollision(const Circle& A, const Circle& B);
    std::tuple<bool, Collision, Collision> IsCollision(const Square& A, const Circle& B);

}