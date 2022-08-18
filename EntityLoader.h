#pragma once

#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <regex>

#include "AllComponents.h"
#include "EntityManager.h"
#include "ModelImporter.h"

#include "Singleton.h"
#include "SimpleTextProcessor.h"
#include "Logger.h"
#include "GLMaterial.h"
#include "SharedGraphicsResources.h"

#include "Input.h"
#include "StringToGLFWKey.h"

class EntityLoader
{
private:
	std::string Path;

public:
	EntityLoader(std::string dataPath): Path(dataPath) {}
	
	void Load();
	void LoadKeyFromFile(std::filesystem::path path);
	
};