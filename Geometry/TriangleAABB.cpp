#include "Triangle.h"
#include "TriangleAABB.h"
#include "PlaneAABB.h"
#include <cassert>

namespace GeometryLib
{
	bool TriangleIntersectsAABB(const Triangle& triangle, const vector4& min, const vector4& max)
	{
		// Get box center and half length.
		vector4 center;
		vector4_add(min, max, center);
		vector4_scale(center, 0.5f, center);

		vector4 halfWidths;
		vector4_sub(max, min, halfWidths);
		vector4_scale(halfWidths, 0.5f, halfWidths);

		const float halfWidthX = halfWidths.extractX();
		const float halfWidthY = halfWidths.extractY();
		const float halfWidthZ = halfWidths.extractZ();

		// Calculate triangle vertices relative to the AABB.
		vector4 triangleVerts[3];
		for (int i = 0; i < 3; i++)
			vector4_sub(triangle.m_Vertices[i].m_Position, center, triangleVerts[i]);

		// Now determine if the AABBs intersect along the cardinal axes.
		{
			// Calculate AABB of the triangle.
			float triangleAABBMinX = FLT_MAX;
			float triangleAABBMinY = FLT_MAX;
			float triangleAABBMinZ = FLT_MAX;

			float triangleAABBMaxX = -FLT_MAX;
			float triangleAABBMaxY = -FLT_MAX;
			float triangleAABBMaxZ = -FLT_MAX;

			for (int i = 0; i < 3; i++)
			{
				float currentX = triangleVerts[i].extractX();
				float currentY = triangleVerts[i].extractY();
				float currentZ = triangleVerts[i].extractZ();

				if (currentX < triangleAABBMinX) triangleAABBMinX = currentX;
				if (currentY < triangleAABBMinY) triangleAABBMinY = currentY;
				if (currentZ < triangleAABBMinZ) triangleAABBMinZ = currentZ;

				if (currentX > triangleAABBMaxX) triangleAABBMaxX = currentX;
				if (currentY > triangleAABBMaxY) triangleAABBMaxY = currentY;
				if (currentZ > triangleAABBMaxZ) triangleAABBMaxZ = currentZ;
			}

			if (triangleAABBMaxX < -halfWidthX) return false;
			if (triangleAABBMinX > halfWidthX) return false;

			if (triangleAABBMaxY < -halfWidthY) return false;
			if (triangleAABBMinY > halfWidthY) return false;

			if (triangleAABBMaxZ < -halfWidthZ) return false;
			if (triangleAABBMinZ > halfWidthZ) return false;
		}

		vector4 edge0;
		vector4_sub(triangleVerts[1], triangleVerts[0], edge0);

		vector4 edge1;
		vector4_sub(triangleVerts[2], triangleVerts[1], edge1);

		vector4 edge2;
		vector4_sub(triangleVerts[0], triangleVerts[2], edge2);

		// Now determine if the triangle intersects the AABB along it's normal.
		{
			// Calculate the normal of the triangle.
			vector4 triangleNormal;

			vector4_crossProduct(edge1, edge0, triangleNormal);
			vector4_normalize(triangleNormal);

			plane intersectionPlane;
			intersectionPlane.setNormal(triangleNormal);
			intersectionPlane.setPointOnPlane(triangleVerts[0]);

			if (!AABBIntersectsPlaneHalfWidths(vector4(0.0f, 0.0f, 0.0f, 1.0f), halfWidths, intersectionPlane))
				return false;
		}

		// As per the Separating axis theorem, we need to determine if the polyhedra can be separated by an
		// axis parallel to the cross product of an edge from the AABB and an edge from the triangle.
		{
			const float v0x = triangleVerts[0].extractX();
			const float v0y = triangleVerts[0].extractY();
			const float v0z = triangleVerts[0].extractZ();

			const float v1x = triangleVerts[1].extractX();
			const float v1y = triangleVerts[1].extractY();
			const float v1z = triangleVerts[1].extractZ();

			const float v2x = triangleVerts[2].extractX();
			const float v2y = triangleVerts[2].extractY();
			const float v2z = triangleVerts[2].extractZ();

			// Edge 0.
			{
				const float e0x = edge0.extractX();
				const float e0y = edge0.extractY();
				const float e0z = edge0.extractZ();

				float fex = fabs(e0x);
				float fey = fabs(e0y);
				float fez = fabs(e0z);

				// Axis 0 and triangle edge 0.
				{
					// Project triangle vertices onto the axis.
					float p0 = e0z * v0y - e0y * v0z;
					float p2 = e0z * v2y - e0y * v2z;

					float minProjection = (p0 < p2) ? p0 : p2;
					float maxProjection = (p0 > p2) ? p0 : p2;

					// Project the box onto the axis as well.
					float radius = (halfWidthY * fez) + (halfWidthZ * fey);

					if (minProjection > radius || maxProjection < -radius)
						return false;
				}

				// Axis 1 and triangle edge 0.
				{
					// Project triangle vertices onto the axis.
					float p0 = -e0z * v0x + e0x * v0z;
					float p2 = -e0z * v2x + e0x * v2z;

					float minProjection = (p0 < p2) ? p0 : p2;
					float maxProjection = (p0 > p2) ? p0 : p2;

					// Project the box onto the axis as well.
					float radius = (halfWidthX * fez) + (halfWidthZ * fex);

					if (minProjection > radius || maxProjection < -radius)
						return false;
				}

				// Axis 2 and triangle edge 0.
				{
					// Project triangle vertices onto the axis.
					float p1 = e0y * v1x - e0x * v1y;
					float p2 = e0y * v2x - e0x * v2y;

					float minProjection = (p1 < p2) ? p1 : p2;
					float maxProjection = (p1 > p2) ? p1 : p2;

					// Project the box onto the axis as well.
					float radius = (halfWidthX * fey) + (halfWidthY * fex);

					if (minProjection > radius || maxProjection < -radius)
						return false;
				}
			}

			// Edge 1.
			{
				const float e1x = edge1.extractX();
				const float e1y = edge1.extractY();
				const float e1z = edge1.extractZ();

				float fex = fabs(e1x);
				float fey = fabs(e1y);
				float fez = fabs(e1z);

				// Axis 0 and triangle edge 1.
				{
					// Project triangle vertices onto the axis.
					float p0 = e1z * v0y - e1y * v0z;
					float p2 = e1z * v2y - e1y * v2z;

					float minProjection = (p0 < p2) ? p0 : p2;
					float maxProjection = (p0 > p2) ? p0 : p2;

					// Project the box onto the axis as well.
					float radius = (halfWidthY * fez) + (halfWidthZ * fey);

					if (minProjection > radius || maxProjection < -radius)
						return false;
				}

				// Axis 1 and triangle edge 1.
				{
					// Project triangle vertices onto the axis.
					float p0 = -e1z * v0x + e1x * v0z;
					float p2 = -e1z * v2x + e1x * v2z;

					float minProjection = (p0 < p2) ? p0 : p2;
					float maxProjection = (p0 > p2) ? p0 : p2;

					// Project the box onto the axis as well.
					float radius = (halfWidthX * fez) + (halfWidthZ * fex);

					if (minProjection > radius || maxProjection < -radius)
						return false;
				}

				// Axis 2 and triangle edge 1.
				{
					// Project triangle vertices onto the axis.
					float p0 = e1y * v0x - e1x * v0y;
					float p1 = e1y * v1x - e1x * v1y;

					float minProjection = (p0 < p1) ? p0 : p1;
					float maxProjection = (p0 > p1) ? p0 : p1;

					// Project the box onto the axis as well.
					float radius = (halfWidthX * fey) + (halfWidthY * fex);

					if (minProjection > radius || maxProjection < -radius)
						return false;
				}
			}

			// Edge 2.
			{
				const float e2x = edge2.extractX();
				const float e2y = edge2.extractY();
				const float e2z = edge2.extractZ();

				float fex = fabs(e2x);
				float fey = fabs(e2y);
				float fez = fabs(e2z);

				// Axis 0 and triangle edge 2.
				{
					// Project triangle vertices onto the axis.
					float p0 = e2z * v0y - e2y * v0z;
					float p1 = e2z * v1y - e2y * v1z;

					float minProjection = (p0 < p1) ? p0 : p1;
					float maxProjection = (p0 > p1) ? p0 : p1;

					// Project the box onto the axis as well.
					float radius = (halfWidthY * fez) + (halfWidthZ * fey);

					if (minProjection > radius || maxProjection < -radius)
						return false;
				}

				// Axis 1 and triangle edge 2.
				{
					// Project triangle vertices onto the axis.
					float p0 = -e2z * v0x + e2x * v0z;
					float p1 = -e2z * v1x + e2x * v1z;

					float minProjection = (p0 < p1) ? p0 : p1;
					float maxProjection = (p0 > p1) ? p0 : p1;

					// Project the box onto the axis as well.
					float radius = (halfWidthX * fez) + (halfWidthZ * fex);

					if (minProjection > radius || maxProjection < -radius)
						return false;
				}

				// Axis 2 and triangle edge 2.
				{
					// Project triangle vertices onto the axis.
					float p1 = e2y * v1x - e2x * v1y;
					float p2 = e2y * v2x - e2x * v2y;

					float minProjection = (p1 < p2) ? p1 : p2;
					float maxProjection = (p1 > p2) ? p1 : p2;

					// Project the box onto the axis as well.
					float radius = (halfWidthX * fey) + (halfWidthY * fex);

					if (minProjection > radius || maxProjection < -radius)
						return false;
				}
			}
		}

		// All attempts to separate the triangle and AABB have failed, therefore they intersect.
		return true;
	}

	void GeometryLib::ComputeAABBForTriangles(unsigned int numTriangles, const Triangle* triangles, vector4& min, vector4& max)
	{
		const float EPSILON = 0.00f;

		assert(numTriangles > 0);

		float maxX = -FLT_MAX;
		float maxY = -FLT_MAX;
		float maxZ = -FLT_MAX;

		float minX = FLT_MAX;
		float minY = FLT_MAX;
		float minZ = FLT_MAX;

		// Determine the global AABB for the kd tree root node.
		for (unsigned int i = 0; i < numTriangles; i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				const vector4& currentPosition = triangles[i].m_Vertices[j].m_Position;
				float x = currentPosition.extractX();
				float y = currentPosition.extractY();
				float z = currentPosition.extractZ();

				if (x > maxX) maxX = x;
				if (x < minX) minX = x;

				if (y > maxY) maxY = y;
				if (y < minY) minY = y;

				if (z > maxZ) maxZ = z;
				if (z < minZ) minZ = z;
			}
		}

		min.setXYZ(minX - EPSILON, minY - EPSILON, minZ - EPSILON);
		max.setXYZ(maxX + EPSILON, maxY + EPSILON, maxZ + EPSILON);
	}


}