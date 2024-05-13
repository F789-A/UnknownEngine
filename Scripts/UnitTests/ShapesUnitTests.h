#pragma once
#include "Physics\Shapes.h"

namespace UnitTests
{

	struct ShapesPolygonIntersectsTestInstance
	{
		std::vector<glm::vec2> A;
		std::vector<glm::vec2> B;
		bool expected;
	};

	struct ShapesPolygonIntersectsTestRunner
	{
		ShapesPolygonIntersectsTestRunner();

		std::vector<ShapesPolygonIntersectsTestInstance> tests;
	};

}
