#include <gtest\gtest.h>
#include <Geometry.h>
#include <MathLib.h>

using namespace GeometryLib;
using namespace MathLib;

TEST(TriangleAABB, TriangleAABB_Intersection_Test_Works)
{
	// Perform a set of triangle AABB intersection tests.
	{
		vector4 minExtents(-1.0f, -1.0f, -1.0f, 1.0f);
		vector4 maxExtents(1.0f, 1.0f, 1.0f, 1.0f);

		{
			// All points outside AABB, but still intersecting.
			Triangle triangle;
			triangle.m_Vertices[0].m_Position.setXYZW(-2.0f, 0.0f, 0.0f, 1.0f);
			triangle.m_Vertices[1].m_Position.setXYZW(2.0f, 0.0f, 0.0f, 1.0f);
			triangle.m_Vertices[2].m_Position.setXYZW(0.0f, 0.0f, 2.0f, 1.0f);
			ASSERT_TRUE(TriangleIntersectsAABB(triangle, minExtents, maxExtents));
		}

		{
			// All points inside AABB.
			Triangle triangle;
			triangle.m_Vertices[0].m_Position.setXYZW(-0.5f, 0.0f, -0.5f, 1.0f);
			triangle.m_Vertices[1].m_Position.setXYZW(0.5f, 0.5f, -0.5f, 1.0f);
			triangle.m_Vertices[2].m_Position.setXYZW(0.0f, 0.0f, 0.5f, 1.0f);
			ASSERT_TRUE(TriangleIntersectsAABB(triangle, minExtents, maxExtents));
		}

		{
			// All points to the right of the AABB.
			Triangle triangle;
			triangle.m_Vertices[0].m_Position.setXYZW(2.0f, 0.0f, -0.5f, 1.0f);
			triangle.m_Vertices[1].m_Position.setXYZW(3.0f, 0.0f, -0.5f, 1.0f);
			triangle.m_Vertices[2].m_Position.setXYZW(2.5f, 0.0f, 0.5f, 1.0f);
			ASSERT_FALSE(TriangleIntersectsAABB(triangle, minExtents, maxExtents));
		}

		{
			// All points to the left of the AABB.
			Triangle triangle;
			triangle.m_Vertices[0].m_Position.setXYZW(-3.0f, 0.0f, -0.5f, 1.0f);
			triangle.m_Vertices[1].m_Position.setXYZW(-2.0f, 0.0f, -0.5f, 1.0f);
			triangle.m_Vertices[2].m_Position.setXYZW(-2.5f, 0.0f, 0.5f, 1.0f);
			ASSERT_FALSE(TriangleIntersectsAABB(triangle, minExtents, maxExtents));
		}

		{
			// All points above the AABB.
			Triangle triangle;
			triangle.m_Vertices[0].m_Position.setXYZW(-0.5f, 4.0f, -0.5f, 1.0f);
			triangle.m_Vertices[1].m_Position.setXYZW(0.5f, 4.0f, -0.5f, 1.0f);
			triangle.m_Vertices[2].m_Position.setXYZW(0.0f, 4.0f, 0.5f, 1.0f);
			ASSERT_FALSE(TriangleIntersectsAABB(triangle, minExtents, maxExtents));
		}

		{
			// All points below the AABB.
			Triangle triangle;
			triangle.m_Vertices[0].m_Position.setXYZW(-0.5f, -4.0f, -0.5f, 1.0f);
			triangle.m_Vertices[1].m_Position.setXYZW(0.5f, -4.0f, -0.5f, 1.0f);
			triangle.m_Vertices[2].m_Position.setXYZW(0.0f, -4.0f, 0.5f, 1.0f);
			ASSERT_FALSE(TriangleIntersectsAABB(triangle, minExtents, maxExtents));
		}

		// All points to the front of the AABB.
		{
			Triangle triangle;
			triangle.m_Vertices[0].m_Position.setXYZW(-0.5f, 0.0f, 1.5f, 1.0f);
			triangle.m_Vertices[1].m_Position.setXYZW(0.5f, 0.5f, 1.5f, 1.0f);
			triangle.m_Vertices[2].m_Position.setXYZW(0.0f, 0.0f, 3.0f, 1.0f);
			ASSERT_FALSE(TriangleIntersectsAABB(triangle, minExtents, maxExtents));
		}

		// All points to the rear of the AABB.
		{
			Triangle triangle;
			triangle.m_Vertices[0].m_Position.setXYZW(-0.5f, 0.0f, -3.0f, 1.0f);
			triangle.m_Vertices[1].m_Position.setXYZW(0.5f, 0.5f, -3.0f, 1.0f);
			triangle.m_Vertices[2].m_Position.setXYZW(0.0f, 0.0f, -1.5f, 1.0f);
			ASSERT_FALSE(TriangleIntersectsAABB(triangle, minExtents, maxExtents));
		}

		// One point inside the AABB, two outside.
		{
			Triangle triangle;
			triangle.m_Vertices[0].m_Position.setXYZW(0.0f, 0.0f, 0.0f, 1.0f);
			triangle.m_Vertices[1].m_Position.setXYZW(10.0f, 0.0f, 0.0f, 1.0f);
			triangle.m_Vertices[2].m_Position.setXYZW(10.0f, 0.0f, -10.0f, 1.0f);
			ASSERT_TRUE(TriangleIntersectsAABB(triangle, minExtents, maxExtents));
		}
	}

	// Test AABB that is not on the origin.
	{
		vector4 offsetMinExtents(0.0f, 0.0f, 0.0f, 0.0f);
		vector4 offsetMaxExtents(10.0f, 10.0f, 10.0f, 0.0f);

		// All points outside the AABB.
		{
			Triangle triangle;
			triangle.m_Vertices[0].m_Position.setXYZW(11.0f, 5.0f, 5.0f, 1.0f);
			triangle.m_Vertices[1].m_Position.setXYZW(15.0f, 11.0f, 5.0f, 1.0f);
			triangle.m_Vertices[2].m_Position.setXYZW(15.0f, 5.0f, 5.0f, 1.0f);
			ASSERT_FALSE(TriangleIntersectsAABB(triangle, offsetMinExtents, offsetMaxExtents));
		}

		// One point inside, two outside.
		{
			Triangle triangle;
			triangle.m_Vertices[0].m_Position.setXYZW(5.0f, 5.0f, 5.0f, 1.0f);
			triangle.m_Vertices[1].m_Position.setXYZW(11.0f, 10.0f, 5.0f, 1.0f);
			triangle.m_Vertices[2].m_Position.setXYZW(11.0f, 5.0f, 5.0f, 1.0f);
			ASSERT_TRUE(TriangleIntersectsAABB(triangle, offsetMinExtents, offsetMaxExtents));
		}
	}

	// This case should fail.
	{
		vector4 minExtents(-0.0516886f, 0.983122f, 0.0f, 0.0f);
		vector4 maxExtents(0.0238535f, 1.03794f, 1.0f, 0.0f);
		 
		Triangle triangle;
		triangle.m_Vertices[0].m_Position.setXYZW(-0.138278f, 1.11092f, 0.5f, 1.0f);
		triangle.m_Vertices[1].m_Position.setXYZW(-0.042796f, 0.91521f, 0.5f, 1.0f);
		triangle.m_Vertices[2].m_Position.setXYZW(-0.079721f, 0.856389f, 0.5f, 1.0f);
		ASSERT_FALSE(TriangleIntersectsAABB(triangle, minExtents, maxExtents));
	}
}