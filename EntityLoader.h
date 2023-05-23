#pragma once
#include <filesystem>
//#include "EcsSystem.h"
//#include "Input.h"

class TagController;
namespace ecs { class EntityManager; };
class Input;

namespace SerializationSystem
{
	void LoadEntity(ecs::EntityManager& em, std::filesystem::path path);
	void LoadEntity(ecs::EntityManager& em, std::filesystem::path path, TagController* tagController);
	void LoadKeyFromFile(Input&, std::filesystem::path);
}