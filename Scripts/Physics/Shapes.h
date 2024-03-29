#pragma once
#include <glm\glm.hpp>
#include <array>
#include <optional>
#include <memory>
#include "Utils\SimpleMath.h"

namespace physics
{ 
    struct Collision
    {
        glm::vec2 pos;
        glm::vec2 normal;
        float penetration;
    };

    struct Intersect
    {
        glm::vec2 firstIntersect;
        std::optional<glm::vec2> secondIntersect;
    };
    
    struct Point;

    struct Shape1D
    {
        std::optional<Intersect> GetIntersect(const Shape1D&); // virtual table
        //virtual bool GetPointIn(const Point& point) = 0; unused
    };

    struct Shape
    {
        std::optional<Collision> GetCollision(const Shape& shape) const; // virtual table
        //std::optional<Intersect> GetIntersect(const Shape1D& shape) const; // virtual table
        virtual bool IntersectWith(const Point& point) const = 0;

        virtual glm::vec2 Center() const = 0;
        virtual float Size() const = 0;
    };

    struct Point
    {
        glm::vec2 origin;
    };

    struct Line
    {
        Line(const glm::vec2& origin, const glm::vec2& direction);

        float A;
        float B;
        float C;

        std::optional<glm::vec2> IntersectWith(const Line& other) const;
    };

    struct Ray
    {
        glm::vec2 origin;
        glm::vec2 direction;
    };

    struct Interval
    {
        glm::vec2 start;
        glm::vec2 end;

        std::optional<glm::vec2> IntersectWith(const Ray& shape) const;
    };

    struct Square : public Shape
    {
        Square(const glm::vec2& min, const glm::vec2& max);

        glm::vec2 min;
        glm::vec2 max;

        glm::vec2 Center() const override;
        float Size() const override;

        bool IntersectWith(const Point& shape) const override;
        bool IntersectWith(const Ray& shape) const;
    };

    struct Circle : public Shape
    {
        Circle(glm::vec2 origin, float radius);
        
        glm::vec2 origin;
        float radius;

        glm::vec2 Center() const override;
        float Size() const override;

        bool IntersectWith(const Point& shape) const override;
        bool IntersectWith(const Ray& shape) const;
    };

    std::optional<std::pair<Collision, Collision>> IsCollision(const Square& A, const Square& B);
    std::optional<std::pair<Collision, Collision>> IsCollision(const Circle& A, const Circle& B);
    std::optional<std::pair<Collision, Collision>> IsCollision(const Square& A, const Circle& B);
    std::optional<std::pair<Collision, Collision>> IsCollision(const Circle& A, const Square& B);

    std::unique_ptr<Shape> ApplyTransformToShape(const Shape& shape, const glm::vec2& position, const glm::vec2& scale);
}