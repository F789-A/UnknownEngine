#pragma once

#include <memory>
#include <utility>
#include <bitset>
#include <string>
#include <glm/glm.hpp>

#include "ECS\EcsSystem.h"

#include "Utils\SimpleTextProcessor.h"
#include "Physics\Shapes.h"

namespace physics
{

    struct Collider : public ecs::Component<Collider>
    {
        static constexpr std::string_view ComponentName = "Collider";


        std::unique_ptr<Shape> shape;

        static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
        {
            auto& collider = em.GetComponent<Collider>(ent);
            glm::vec2 pos = TextTools::ReadVec2(res["Position"]);
            if (res["shape"] == "Circle")
            {
                float radius = std::stof(res["rad"]);
                collider.shape = std::make_unique<Circle>(pos, radius);
            }
            else if (res["shape"] == "Square")
            {
                float len = std::stof(res["len"]);
                collider.shape = std::make_unique<Square>(pos + glm::vec2(-len / 2.0f, -len / 2.0f), 
                    pos + glm::vec2(len / 2.0f, len / 2.0f));
            }
        }
    };
}