#include "ShapesUnitTests.h"

UnitTests::ShapesPolygonIntersectsTestRunner::ShapesPolygonIntersectsTestRunner()
{
	tests =
	{
		{{ {4,0}, {2,2}, {0,0} }, { {6, 1}, {4, 3}, {2, 1} }, true},
		{{ {4,0}, {2,2}, {0,0} }, { {10, 1}, {8, 3}, {6, 1} }, false}
	};

	for (auto& testExs : tests)
	{
		auto a = physics::Polygon(testExs.A);
		auto b = physics::Polygon(testExs.B);

		if (physics::IsCollision(a, b).has_value() != testExs.expected) 
		{
			throw "Unit test failed";
		}
	}
}