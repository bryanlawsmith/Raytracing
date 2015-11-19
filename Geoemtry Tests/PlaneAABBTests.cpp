#include <gtest\gtest.h>
#include <Geometry.h>
#include <MathLib.h>

using namespace GeometryLib;
using namespace MathLib;

TEST(PlaneAABB, PlaneAABB_Intersection_Test_Works)
{
	vector4 center(1.0f, 1.0f, 1.0f, 1.0f);
	vector4 widths(0.5f, 0.5f, 0.5f, 0.0f);

	// AABB behind the plane.
	{
		vector4 planePoint(2.0f, 2.0f, 2.0f, 1.0f);
		vector4 planeNormal(1.0f, 1.0f, 1.0f, 0.0f);
		vector4_normalize(planeNormal);

		plane intersectionPlane;
		intersectionPlane.setPointOnPlane(planePoint);
		intersectionPlane.setNormal(planeNormal);

		ASSERT_FALSE(GeometryLib::AABBIntersectsPlaneHalfWidths(center, widths, intersectionPlane));
	}

	// AABB in front of the plane.
	{
		vector4 planePoint(-2.0f, -2.0f, -2.0f, 1.0f);
		vector4 planeNormal(-1.0f, -1.0f, -1.0f, 0.0f);
		vector4_normalize(planeNormal);

		plane intersectionPlane;
		intersectionPlane.setPointOnPlane(planePoint);
		intersectionPlane.setNormal(planeNormal);

		ASSERT_FALSE(GeometryLib::AABBIntersectsPlaneHalfWidths(center, widths, intersectionPlane));
	}

	// Plane intersects AABB.
	{
		vector4 planePoint(1.0f, 1.0f, 1.0f, 1.0f);
		vector4 planeNormal(1.0f, 1.0f, 1.0f, 0.0f);
		vector4_normalize(planeNormal);

		plane intersectionPlane;
		intersectionPlane.setPointOnPlane(planePoint);
		intersectionPlane.setNormal(planeNormal);

		ASSERT_TRUE(GeometryLib::AABBIntersectsPlaneHalfWidths(center, widths, intersectionPlane));

		planeNormal.setXYZW(-1.0f, -1.0f, -1.0f, 0.0f);
		vector4_normalize(planeNormal);
		intersectionPlane.setNormal(planeNormal);

		ASSERT_TRUE(GeometryLib::AABBIntersectsPlaneHalfWidths(center, widths, intersectionPlane));
	}
}