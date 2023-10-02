#include "Bvh.h"

#include <glm/gtx/norm.hpp>

namespace Physics
{
    Bvh2D::BvhNode::BvhNode(BvhNode* parent, const Square& volume): parent(parent), volume(volume)
    {}

    Bvh2D::Bvh2D(const Square& initSpace)
    {
        root = std::make_unique<BvhNode>(nullptr, initSpace);
    }
    
    void Bvh2D::Insert(const Circle& object)
    {
        InsertFromNode(root.get(), object);
    }

    void Bvh2D::InsertFromNode(BvhNode* root, const Circle& object)
    {
        if (!root->volume.IntersectWith(Point{ object.origin }))
        {
            //alert: out of bounds;
            return;
        }

        BvhNode* current = root;

        while (true)
        {
            if (glm::length2(current->volume.max - current->volume.min) < minSize * minSize)
            {
                current->objects.push_back(object);
                return;
            }

            if (current->objects.size() == 1)
            {
                // remove this
                InsertFromNode(current, current->objects[0]); // not recursive (only one call in callstack)
            }

            glm::vec2 downCenter = (current->volume.min + glm::vec2(current->volume.max.x, current->volume.min.y)) / 2.0f;
            glm::vec2 leftCenter = (current->volume.min + glm::vec2(current->volume.min.x, current->volume.max.y)) / 2.0f;

            Ray horizontalRay{ leftCenter, {1.0f, 0.0f} };
            Ray verticalRay{ downCenter, {0.0f, 1.0f} };

            if (object.IntersectWith(horizontalRay) || object.IntersectWith(verticalRay))
            {
                current->objects.push_back(object);
            }
            else
            {
                glm::vec2 center = current->volume.Center();
                int numberOfVolume = object.origin.x > center.x + 2 * object.origin.y < center.y; // see figure
                if (current->childs[numberOfVolume])
                {
                    current = current->childs[numberOfVolume].get();
                    continue;
                }
                else
                {
                    float left = current->volume.min.x;
                    float right = current->volume.max.x;
                    float up = current->volume.max.y;
                    float down = current->volume.min.y;
                    glm::vec2 newVolumeMin;
                    glm::vec2 newVolumeMax;
                    if (numberOfVolume == 0 || numberOfVolume == 2)
                    {
                        newVolumeMin.x = left;
                        newVolumeMax.x = (right + left) / 2.0f;
                    }
                    else
                    {
                        newVolumeMin.x = (left + right) / 2.0f;
                        newVolumeMax.x = right;
                    }
                    if (numberOfVolume == 2 || numberOfVolume == 3)
                    {
                        newVolumeMin.y = down;
                        newVolumeMax.y = (down + up) / 2.0f;
                    }
                    else
                    {
                        newVolumeMin.y = (down + up) / 2.0f;
                        newVolumeMax.y = up;
                    }
                    current->childs[numberOfVolume] = std::make_unique<BvhNode>(current, Square{ newVolumeMin, newVolumeMax });
                    current->childs[numberOfVolume]->objects.push_back(object);
                }
            }
        }
    }
}