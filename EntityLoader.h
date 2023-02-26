#pragma once
#include <filesystem>
#include "ecs_EntityManager.h"
#include "Input.h"

namespace SerializationSystem
{
	void LoadEntity(ecs::EntityManager& em, std::filesystem::path path);
	void LoadKeyFromFile(Input&, std::filesystem::path);
}