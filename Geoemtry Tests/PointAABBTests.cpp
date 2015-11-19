#include <gtest\gtest.h>
#include <Geometry.h>
#include <MathLib.h>

using namespace GeometryLib;
using namespace MathLib;

TEST(PointAABB, PointAABB_Intersection_Test_Works)
{
	vector4 center(1.0f, 1.0f, 1.0f, 1.0f);
	vector4 widths(0.5f, 0.5f, 0.5f, 0.0f);

	// Point inside.
	{
		vector4 point(1.4f, 1.4f, 1.2f, 1.0f);
		ASSERT_TRUE(GeometryLib::PointInsideAABB(point, center, widths));
	}

	// Point outside.
	{
		vector4 point(1.5f, 1.6f, 1.2f, 1.0f);
		ASSERT_FALSE(GeometryLib::PointInsideAABB(point, center, widths));
	}
}