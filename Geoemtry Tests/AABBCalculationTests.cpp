#include <gtest\gtest.h>
#include <Geometry.h>
#include <MathLib.h>

using namespace GeometryLib;
using namespace MathLib;

TEST(TriangleAABB, Calculate_AABB_For_Triangles_Works)
{
	Triangle triangles[3];

	triangles[0].m_Vertices[0].m_Position.setXYZW(0.0f, 0.0f, 0.0f, 1.0f);
	triangles[0].m_Vertices[1].m_Position.setXYZW(10.0f, 0.0f, 0.0f, 1.0f);
	triangles[0].m_Vertices[2].m_Position.setXYZW(1.0f, 1.0f, 1.0f, 1.0f);

	triangles[1].m_Vertices[0].m_Position.setXYZW(-10.0f, 10.0f, -10.0f, 1.0f);
	triangles[1].m_Vertices[1].m_Position.setXYZW(1.0f, 1.0f, 1.0f, 1.0f);
	triangles[1].m_Vertices[2].m_Position.setXYZW(0.0f, 0.0f, 0.0f, 1.0f);

	triangles[2].m_Vertices[0].m_Position.setXYZW(0.0f, -10.0f, 0.0f, 1.0f);
	triangles[2].m_Vertices[1].m_Position.setXYZW(-1.0f, 0.0f, 10.0f, 1.0f);
	triangles[2].m_Vertices[2].m_Position.setXYZW(1.0f, 0.0f, 0.0f, 1.0f);

	vector4 minExtents;
	vector4 maxExtents;

	GeometryLib::ComputeAABBForTriangles(3, triangles, minExtents, maxExtents);

	ASSERT_TRUE(maxExtents.extractX() == 10.0f);
	ASSERT_TRUE(maxExtents.extractY() == 10.0f);
	ASSERT_TRUE(maxExtents.extractZ() == 10.0f);

	ASSERT_TRUE(minExtents.extractX() == -10.0f);
	ASSERT_TRUE(minExtents.extractY() == -10.0f);
	ASSERT_TRUE(minExtents.extractZ() == -10.0f);
}