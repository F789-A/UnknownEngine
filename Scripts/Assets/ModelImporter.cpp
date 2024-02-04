#include "ModelImporter.h"

Model ImportModel(std::string pathS)
{
	std::filesystem::path path(pathS);
	if (path.extension() == ".obj")
	{
		return ImportObj(path);
	}

	throw "Not supported format";
}