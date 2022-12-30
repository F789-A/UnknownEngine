#pragma once

#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>

#include "ModelImporter.h"

#include "Singleton.h"
#include "SimpleTextProcessor.h"
#include "Logger.h"
#include "GLMaterial.h"
#include "SharedGraphicsResources.h"

#include "Input.h"
#include "StringToGLFWKey.h"
#include <vector>
#include <tuple>

namespace SerializationSystem
{
	void LoadEntity(std::filesystem::path path);
	void LoadKeyFromFile(std::filesystem::path path);
}