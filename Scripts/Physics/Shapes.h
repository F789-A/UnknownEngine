#pragma once
#include <glm\glm.hpp>
#include <array>
#include <optional>
#include <vector>
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
    
    using Point = glm::vec2;

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

        std::optional<glm::vec2> IntersectWith(const Line& line) const;
        std::optional<glm::vec2> IntersectWith(const Ray& ray) const;
    };

    struct Interval
    {
        glm::vec2 start;
        glm::vec2 end;

        std::optional<glm::vec2> IntersectWith(const Line& line) const;
        std::optional<glm::vec2> IntersectWith(const Ray& ray) const;
        std::optional<glm::vec2> IntersectWith(const Interval& interval) const;
    };

    struct Shape
    {
        std::optional<Collision> GetCollision(const Shape& shape) const; // virtual table
        virtual bool IntersectWith(const Point& point) const = 0;

        virtual glm::vec2 Center() const = 0;
        virtual float Size() const = 0;
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

        glm::vec2 GetFarthestPoint(const glm::vec2& dir) const;
    };

    struct GJKPoint
    {
        glm::vec2 pos;
        int pA;
        int pB;
    };

    struct GJKSimplex
    {
        static constexpr int StackSize = 3;

        std::array<GJKPoint, StackSize> points;
        int size = 0;

        GJKSimplex& operator=(const std::initializer_list<GJKPoint>& list);
        void Push(const GJKPoint& point);
        GJKPoint& operator[](int i);
        GJKPoint operator[](int i) const;
    };

    struct Polygon : public Shape
    {
        Polygon(const std::vector<glm::vec2>& vert);

        std::vector<glm::vec2> vertices;

        glm::vec2 Center() const override;
        float Size() const override;

        bool IntersectWith(const Point& shape) const override;
        bool IntersectWith(const Ray& shape) const;

        int GetFarthestPointIndex(const glm::vec2& dir) const;
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

    std::pair<std::optional<glm::vec2>, std::optional<glm::vec2>> Intersect(const Interval& interval, const Circle& circle);

    std::optional<Collision> IsCollision(const Square& A, const Square& B);
    std::optional<Collision> IsCollision(const Circle& A, const Circle& B);
    std::optional<Collision> IsCollision(const Square& A, const Circle& B);
    std::optional<Collision> IsCollision(const Circle& A, const Square& B);
    
    std::optional<Collision> IsCollision(const Polygon& A, const Circle& B);
    std::optional<Collision> IsCollision(const Polygon& A, const Polygon& B);


    std::unique_ptr<Shape> ApplyTransformToShape(const Shape& shape, const glm::vec2& position, const glm::vec2& scale);
}