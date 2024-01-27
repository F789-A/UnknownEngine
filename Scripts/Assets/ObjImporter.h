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

#include "PolygonGeometry.h"

#include <glm/glm.hpp>
#include "Model.h"
#include "Utils\SimpleTextProcessor.h"

Model ImportObj(std::filesystem::path path);