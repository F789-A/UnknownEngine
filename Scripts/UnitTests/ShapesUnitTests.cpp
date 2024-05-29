#include "ShapesUnitTests.h"

UnitTests::ShapesPolygonIntersectsTestRunner::ShapesPolygonIntersectsTestRunner()
{
	std::vector<ShapesPolygonIntersectsTestInstance> testsPoly =
	{
		{{ {4.0f,0.0f}, {2.0f,2.0f}, {0.0f,0.0f} }, { {6.0f, 1.0f}, {4.0f, 3.0f}, {2.0f, 1.0f} }, true},
		{{ {4.0f,0.0f}, {2.0f,2.0f}, {0.0f,0.0f} }, { {10.0f, 1.0f}, {8.0f, 3.0f}, {6.0f, 1.0f} }, false}
		//{ { {0,-20}, {50,-20}, {70,0}, {50, 20}, {0, 20} }, { {40, -10}, {55, -30}, {80, -30},  {80, 30}, {55, 30}, {40, 10} }, true}
	};

	for (auto& testExs : testsPoly)
	{
		auto a = physics::Polygon(testExs.A);
		auto b = physics::Polygon(testExs.B);

		auto result = physics::IsCollision(a, b);

		if (result.has_value() != testExs.expected)
		{
			throw "Unit test failed";
		}
	}

	std::vector<ShapesPolygonIntersectsTest2Instance> test2
	{
		{physics::Polygon({ {4.0f,0.0f}, {2.0f,2.0f}, {0.0f,0.0f} }), physics::Circle({5.0f, 0.0f}, 2.0f), true},
		{physics::Polygon({ {4.0f,0.0f}, {2.0f,2.0f}, {0.0f,0.0f} }), physics::Circle({7.0f, 0.0f}, 2.0f), false}
	};

	for (auto& testExs : test2)
	{
		if (physics::IsCollision(testExs.A, testExs.B).has_value() != testExs.expected)
		{
			throw "Unit test failed";
		}
	}
}