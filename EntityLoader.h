#pragma once
#include <filesystem>

namespace ecs { class EntityManager; };
class Input;

namespace SerializationSystem
{
	void LoadEntity(ecs::EntityManager& em, std::filesystem::path path);
	void LoadKeyFromFile(Input&, std::filesystem::path);
}