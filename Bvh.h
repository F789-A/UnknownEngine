#pragma once

#include "Shapes.h"
#include <memory>
#include <array>
#include <vector>

namespace Physics
{
    //|-----|-----|
    //|     |     |
    //|  0  |  1  |
    //|     |     |
    //|-----|-----|
    //|     |     |
    //|  2  |  3  |
    //|     |     |
    //|-----|-----|

    //TODO использовать код Мортона
    class Bvh2D
    {
        struct BvhNode
        {
            BvhNode(BvhNode* parent, const Square& volume);

            BvhNode* parent;
            Square volume;

            std::array<std::unique_ptr<BvhNode>, 4> childs;
            std::vector<Circle> objects;
        };

        constexpr static float minSize = 1.0f; // diagonal
        std::unique_ptr<BvhNode> root = nullptr;

    public:

        Bvh2D(const Square& initSpace);

        void Remove();

        void Insert(const Circle& object);

        //std::vector<RigidBody>

    private:
        void InsertFromNode(BvhNode* root, const Circle& object);
    };
}