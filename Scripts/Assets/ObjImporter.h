#pragma once

#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <utility>

#include <list>
#include <map>
#include <array>
#include <vector>

#include <glm/glm.hpp>

#include "PolygonGeometry.h"
#include "Model.h"

#include "Utils\SimpleTextProcessor.h"

Model ImportObj(std::filesystem::path path);