#pragma once

#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <regex>

#include "EntityManager.h"
#include "Transform.h"
#include "ComponentThatAlwaysSayHello.h"
#include "RenderMesh.h"
#include "ModelImporter.h"

#include "GLMaterial.h"
#include "Camera.h"
#include "CameraController.h"

#include "Singleton.h"
#include "SimpleTextProcessor.h"
#include "Logger.h"
#include "SharedGraphicsResources.h"

class EntityLoader
{
	std::string Path;

public:

	EntityLoader(std::string dataPath): Path(dataPath)
	{}

	std::vector<std::string> Split(const std::string& str);
	
	void Load();
	
};