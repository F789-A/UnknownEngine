#pragma once

#include "EntityManagerBase.h"

namespace ec
{
	class IComponent
	{
	public:
		virtual void SetLinkedEntity(EntityManagerBase* manager, int entity) = 0;

		virtual ~IComponent() {};
	};
}