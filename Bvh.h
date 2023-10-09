#pragma once
#include <memory>
#include <array>
#include <vector>
#include <utility>
#include <queue>
#include <glm/gtx/norm.hpp>

#include "Shapes.h"

namespace Physics
{
    template<typename BvhNodeData>
    class Bvh2D
    {
    private:
        struct BvhNode
        {
            BvhNode(BvhNode* parent, const Square& volume);

            BvhNode* parent;
            std::array<std::unique_ptr<BvhNode>, 4> childs;

            Square volume;
            std::vector<std::pair<Circle, BvhNodeData>> objects;
        };

        constexpr static float minSize = 1.0f; // diagonal
        std::unique_ptr<BvhNode> root = nullptr;

    public:

        Bvh2D(const Square& initSpace);

        void Insert(const Circle& object, const BvhNodeData& bvhNodeData);

        void Remove(const Circle& object, const BvhNodeData& bvhNodeData);

        std::vector<std::pair<BvhNodeData, BvhNodeData>> ProcessRawCollision();
    };

    //IMPLEMENTATION

    //|-----|-----|
    //|     |     |
    //|  0  |  1  |
    //|     |     |
    //|-----|-----|
    //|     |     |
    //|  2  |  3  |
    //|     |     |
    //|-----|-----|

    //TODO код Мортона?
    template<typename BvhNodeData>
    Bvh2D<BvhNodeData>::BvhNode::BvhNode(BvhNode* parent, const Square& volume) : parent(parent), volume(volume)
    {}

    template<typename BvhNodeData>
    Bvh2D<BvhNodeData>::Bvh2D(const Square& initSpace)
    {
        root = std::make_unique<BvhNode>(nullptr, initSpace);
    }

    template<typename BvhNodeData>
    std::vector<std::pair<BvhNodeData, BvhNodeData>> Bvh2D<BvhNodeData>::ProcessRawCollision()
    {
        auto collectChilds = [](std::queue<BvhNode*>& queue, BvhNode* root)
        {
            for (auto& next : root->childs)
            {
                if (next)
                {
                    queue.push(next.get());
                }
            }
        };

        std::vector<std::pair<BvhNodeData, BvhNodeData>> result;

        std::queue<BvhNode*> queue;
        queue.push(root.get());

        while (!queue.empty())
        {
            BvhNode* current = queue.front();
            queue.pop();

            collectChilds(queue, current);

            for (int i = 0; i < current->objects.size(); ++i)
            {
                auto obj = current->objects[i];
                for (int j = i; j < current->objects.size(); ++j)
                {
                    if (std::get<0>(IsCollision(obj.first, current->objects[j].first)))
                    {
                        result.push_back(std::make_pair(obj.second, current->objects[j].second));
                    }
                }

                std::queue<BvhNode*> queueBranch;

                collectChilds(queueBranch, current);

                while (!queueBranch.empty())
                {
                    current = queueBranch.front();
                    queueBranch.pop();

                    collectChilds(queueBranch, current);

                    for (auto obj2 : current->objects)
                    {
                        if (std::get<0>(IsCollision(obj.first, obj2.first)))
                        {
                            result.push_back(std::make_pair(obj.second, obj2.second));
                        }
                    }
                }
            }

        }

        return result;
    }

    template<typename BvhNodeData>
    void Bvh2D<BvhNodeData>::Remove(const Circle& object, const BvhNodeData& bvhNodeData)
    {
        auto findAndRemove = [](BvhNode* current, const BvhNodeData& bvhNodeData)
        {
            for (auto it = current->objects.begin(); it != current->objects.end(); ++it)
            {
                if (it->second == bvhNodeData)
                {
                    current->objects.erase(it);
                    return;
                }
            }
        };

        BvhNode* current = root.get();

        while (true)
        {
            if (glm::length2(current->volume.max - current->volume.min) <= minSize * minSize)
            {
                findAndRemove(current, bvhNodeData);
                return;
            }

            glm::vec2 downCenter = (current->volume.min + glm::vec2(current->volume.max.x, current->volume.min.y)) / 2.0f;
            glm::vec2 leftCenter = (current->volume.min + glm::vec2(current->volume.min.x, current->volume.max.y)) / 2.0f;

            Ray horizontalRay{ leftCenter, {1.0f, 0.0f} };
            Ray verticalRay{ downCenter, {0.0f, 1.0f} };

            if (object.IntersectWith(horizontalRay) || object.IntersectWith(verticalRay))
            {
                findAndRemove(current, bvhNodeData);
                return;
            }
            else
            {
                glm::vec2 center = current->volume.Center();
                int numberOfVolume = object.origin.x > center.x + 2 * object.origin.y < center.y; // see figure
                current = current->childs[numberOfVolume].get();
            }
        }
    }

    template<typename BvhNodeData>
    void Bvh2D<BvhNodeData>::Insert(const Circle& object, const BvhNodeData& bvhNodeData)
    {
        if (!root->volume.IntersectWith(Point{ object.origin }))
        {
            throw "bvh: out of bounds";
        }

        BvhNode* current = root.get();

        while (true)
        {
            if (glm::length2(current->volume.max - current->volume.min) <= minSize * minSize)
            {
                current->objects.push_back(std::make_pair(object, bvhNodeData));
                return;
            }

            glm::vec2 downCenter = (current->volume.min + glm::vec2(current->volume.max.x, current->volume.min.y)) / 2.0f;
            glm::vec2 leftCenter = (current->volume.min + glm::vec2(current->volume.min.x, current->volume.max.y)) / 2.0f;

            Ray horizontalRay{ leftCenter, {1.0f, 0.0f} };
            Ray verticalRay{ downCenter, {0.0f, 1.0f} };

            if (object.IntersectWith(horizontalRay) || object.IntersectWith(verticalRay))
            {
                current->objects.push_back(std::make_pair(object, bvhNodeData));
                return;
            }
            else
            {
                glm::vec2 center = current->volume.Center();
                int numberOfVolume = object.origin.x > center.x + 2 * object.origin.y < center.y; // see figure
                if (!current->childs[numberOfVolume])
                {
                    float left = current->volume.min.x;
                    float right = current->volume.max.x;
                    float up = current->volume.max.y;
                    float down = current->volume.min.y;

                    bool leftCol = numberOfVolume == 0 || numberOfVolume == 2;
                    bool downRow = numberOfVolume == 2 || numberOfVolume == 3;

                    glm::vec2 newVolumeMin = { leftCol ? left : (left + right) / 2.0f , downRow ? down : (down + up) / 2.0f };
                    glm::vec2 newVolumeMax = { leftCol ? (right + left) / 2.0f : right, downRow ? (down + up) / 2.0f : up };

                    current->childs[numberOfVolume] = std::make_unique<BvhNode>(current, Square{ newVolumeMin, newVolumeMax });
                }
                current = current->childs[numberOfVolume].get();
            }
        }
    }
}