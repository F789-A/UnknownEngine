#include "ECS\Systems\Systems.h"

#include "ECS\Components\Game\Labyrinth\RoomTraveler.h"
#include "ECS\Components\Game\Labyrinth\RoomRedrawer.h"
#include "ECS\Components\Game\Labyrinth\GraphDebugInfo.h"
#include "ECS\Components\Transform.h"
#include "ECS\Components\Game\Labyrinth\LabyrinthData.h"
#include "ECS\Components\Physics\PhysicalBody.h"
#include "ECS\Components\Physics\Collider.h"
#include "Core\Input.h"
#include "Graphics\GraphicCore.h"

void Labyrinth::RoomTravelerController(ecs::EntityManager& em)
{
	for (auto l = em.GetComponents<RoomTraveler, RoomRedrawer, GraphDebugInfo>(); !l.end(); ++l)
	{
		auto [roomTraveler, redrawer, deb] = *l;

		if (Input::GetInstance().GetButton("MouseLeft", Input::PressMode::Press))
		{
			for (auto l = em.GetComponents<Door, Transform, physics::Collider>(); !l.end(); ++l)
			{
				auto [door, tr, coll] = *l;

				glm::vec2 mousePos = { Input::GetInstance().GetMousePosX(),
					GraphicCore::GetInstance().Height - Input::GetInstance().GetMousePosY() };
				mousePos = mousePos / static_cast<float>(GraphicCore::GetInstance().Width) * 10.0f - glm::vec2(5.0f, 3.75f); // использовать камеру

				auto shape = ApplyTransformToShape(*coll.shape.get(), glm::vec2(tr.Position), glm::vec2(tr.Scale));
				if (shape->IntersectWith(physics::Point{ mousePos }))
				{
					roomTraveler.PreviousRoom = roomTraveler.CurrentRoom;
					roomTraveler.CurrentRoom = door.NextRoom;
					redrawer.NeedRedraw = true;
					deb.forceRedraw = true;
					break;
				}
			}

			for (auto l = em.GetComponents<WinDoor, Transform, physics::Collider>(); !l.end(); ++l)
			{
				auto [door, tr, coll] = *l;

				glm::vec2 mousePos = { Input::GetInstance().GetMousePosX(),
					GraphicCore::GetInstance().Height - Input::GetInstance().GetMousePosY() };
				mousePos = mousePos / static_cast<float>(GraphicCore::GetInstance().Width) * 10.0f - glm::vec2(5.0f, 3.75f); // использовать камеру

				auto shape = ApplyTransformToShape(*coll.shape.get(), glm::vec2(tr.Position), glm::vec2(tr.Scale));
				if (shape->IntersectWith(physics::Point{ mousePos }))
				{
					break;
				}
			}
		}
		
	}
}
