#include "KdTreeStackTraversal.h"
#include "StaticMeshKdTree.h"
#include "kdTreeNode.h"
#include "DebugManager.h"
#include <iostream>
#include <MathLib.h>
#include <Geometry.h>
#include <Color.h>

using namespace std;
using namespace Debugging;
using namespace CommonCode;

namespace Raytracer
{
	struct IntersectionInfo
	{
		const StaticMeshKdTree& m_Mesh;
		const ray& m_Ray;
		unsigned int& m_Index;
		float& m_T;
		float& m_U;
		float& m_V;
	};

	static void RenderDebugInfo(KdTreeNode& node, IntersectionInfo& intersectionInfo)
	{
		auto& debugManager = DebugManager::GetInstance();
		const auto& minExtents = node.GetBoundingMin();
		const auto& maxExtents = node.GetBoundingMax();

		float minX = minExtents.extractX();
		float minY = minExtents.extractY();
		float minZ = minExtents.extractZ();

		float maxX = maxExtents.extractX();
		float maxY = maxExtents.extractY();
		float maxZ = maxExtents.extractZ();

		const auto& normal = node.GetSplittingPlane().getNormal();
		Color color	{ normal.extractX(), normal.extractY(), normal.extractZ(), 1.0f };

		if (color.m_R == 0.0f && color.m_G == 0.0f && color.m_B == 0.0f)
		{
			color.m_R = 1.0f;
			color.m_G = 1.0f;
		}

		GeometryLib::Line line;

		// Top lines.
		{
			line.m_P0.setXYZW(minX, maxY, minZ, 1.0f);
			line.m_P1.setXYZW(maxX, maxY, minZ, 1.0f);
			debugManager.AddLine(line, color);

			line.m_P0.setXYZW(maxX, maxY, minZ, 1.0f);
			line.m_P1.setXYZW(maxX, maxY, maxZ, 1.0f);
			debugManager.AddLine(line, color);

			line.m_P0.setXYZW(maxX, maxY, maxZ, 1.0f);
			line.m_P1.setXYZW(minX, maxY, maxZ, 1.0f);
			debugManager.AddLine(line, color);

			line.m_P0.setXYZW(minX, maxY, maxZ, 1.0f);
			line.m_P1.setXYZW(minX, maxY, minZ, 1.0f);
			debugManager.AddLine(line, color);
		}

		// Bottom lines.
		{
			line.m_P0.setXYZW(minX, minY, minZ, 1.0f);
			line.m_P1.setXYZW(maxX, minY, minZ, 1.0f);
			debugManager.AddLine(line, color);

			line.m_P0.setXYZW(maxX, minY, minZ, 1.0f);
			line.m_P1.setXYZW(maxX, minY, maxZ, 1.0f);
			debugManager.AddLine(line, color);

			line.m_P0.setXYZW(maxX, minY, maxZ, 1.0f);
			line.m_P1.setXYZW(minX, minY, maxZ, 1.0f);
			debugManager.AddLine(line, color);

			line.m_P0.setXYZW(minX, minY, maxZ, 1.0f);
			line.m_P1.setXYZW(minX, minY, minZ, 1.0f);
			debugManager.AddLine(line, color);
		}

		// Connecting lines.
		{
			line.m_P0.setXYZW(minX, minY, minZ, 1.0f);
			line.m_P1.setXYZW(minX, maxY, minZ, 1.0f);
			debugManager.AddLine(line, color);

			line.m_P0.setXYZW(maxX, minY, minZ, 1.0f);
			line.m_P1.setXYZW(maxX, maxY, minZ, 1.0f);
			debugManager.AddLine(line, color);

			line.m_P0.setXYZW(maxX, minY, maxZ, 1.0f);
			line.m_P1.setXYZW(maxX, maxY, maxZ, 1.0f);
			debugManager.AddLine(line, color);

			line.m_P0.setXYZW(minX, minY, maxZ, 1.0f);
			line.m_P1.setXYZW(minX, maxY, maxZ, 1.0f);
			debugManager.AddLine(line, color);
		}

		if (node.IsChild())
		{
			auto numTriangles = node.GetNumTriangles();
			auto triangleIndices = node.GetTriangleList();
			auto triangles = intersectionInfo.m_Mesh.GetTriangles();
			
			for (unsigned i = 0; i < numTriangles; i++)
			{
				// Get box center and half length.
				vector4 center;
				vector4_add(minExtents, maxExtents, center);
				vector4_scale(center, 0.5f, center);

				vector4 halfWidths;
				vector4_sub(maxExtents, minExtents, halfWidths);
				vector4_scale(halfWidths, 0.5f, halfWidths);

				const Triangle& triangle = triangles[triangleIndices[i]];

				bool onePointInside = false;
				for (unsigned j = 0; j < 3; j++)
				{
					if (GeometryLib::PointInsideAABB(triangle.m_Vertices[j].m_Position, center, halfWidths))
					{
						onePointInside = true;
						break;
					}
				}
				
				if (onePointInside)
				{
					color.m_R = 1.0f;
					color.m_G = 1.0f;
					color.m_B = 0.0f;
				}
				else
				{
					color.m_R = 1.0f;
					color.m_G = 0.0f;
					color.m_B = 1.0f;
				}

				vector4_copy(line.m_P0, triangle.m_Vertices[0].m_Position);
				vector4_copy(line.m_P1, triangle.m_Vertices[1].m_Position);
				debugManager.AddLine(line, color);

				vector4_copy(line.m_P0, triangle.m_Vertices[1].m_Position);
				vector4_copy(line.m_P1, triangle.m_Vertices[2].m_Position);
				debugManager.AddLine(line, color);

				vector4_copy(line.m_P0, triangle.m_Vertices[2].m_Position);
				vector4_copy(line.m_P1, triangle.m_Vertices[0].m_Position);
				debugManager.AddLine(line, color);
			}
		}
	}

	static bool IntersectKdTreeChildNode(KdTreeNode& node, IntersectionInfo& intersectionInfo)
	{
		bool intersectionFound = false;
		unsigned int index = 0;
		float t = FLT_MAX;
		float u;
		float v;

		auto triangleList = node.GetTriangleList();
		auto triangles = intersectionInfo.m_Mesh.GetTriangles();
		auto numTriangles = node.GetNumTriangles();

		for (unsigned int i = 0; i < numTriangles; i++)
		{
			float currentT;
			float currentU;
			float currentV;

			if (GeometryLib::RayTriangleIntersection(intersectionInfo.m_Ray, triangles[triangleList[i]], currentT, currentU, currentV))
			{
				if (currentT < t)
				{
					// The triangle is closer to the ray origin than the previous value.
					intersectionFound = true;

					index = i;
					t = currentT;
					u = currentU;
					v = currentV;
				}
			}
		}

		if (intersectionFound)
		{
			intersectionInfo.m_Index = triangleList[index];
			intersectionInfo.m_T = t;
			intersectionInfo.m_U = u;
			intersectionInfo.m_V = v;
		}

		return intersectionFound;
	}

	static bool IntersectKdTreeNode(KdTreeNode& node, IntersectionInfo& intersectionInfo)
	{
		auto& debugManager = DebugManager::GetInstance();
		if (debugManager.GetEnabled())
			RenderDebugInfo(node, intersectionInfo);

		if (node.IsChild())
			return IntersectKdTreeChildNode(node, intersectionInfo);

		float tEntry;		// t value of the ray at the entry point of this node's voxel.
		float tExit;		// t value of the ray at the exit point of this node's voxel.
		float tPlane;		// t value of the ray if it intersects the splitting plane of this node's voxel.

		bool rayIntersectsAABB = GeometryLib::RayIntersectsAABB(intersectionInfo.m_Ray, &tEntry, &tExit, node.GetBoundingMin(), node.GetBoundingMax());
		if (!rayIntersectsAABB)
			return false;

		auto childNodes = node.GetChildren();

		// Determine the index of the near and far sides of the node.
		unsigned int nearSideIndex = 0;
		unsigned int farSideIndex = 1;

		bool pointInPositivePlaneHalf = MathLib::pointOnPositivePlaneSide(node.GetSplittingPlane(), intersectionInfo.m_Ray.getPosition());
		if (pointInPositivePlaneHalf)
		{
			nearSideIndex = 1;
			farSideIndex = 0;
		}

		bool rayIntersectsSplittingPlane = MathLib::intersectRayWithPlane(intersectionInfo.m_Ray, node.GetSplittingPlane(), &tPlane);
		rayIntersectsSplittingPlane = rayIntersectsSplittingPlane && (tPlane > 0.0f);
		if (!rayIntersectsSplittingPlane)
			return IntersectKdTreeNode(*childNodes[nearSideIndex], intersectionInfo);

		// Determine if we need to traverse one or both nodes of the node.
		if (tExit < tPlane)
			return IntersectKdTreeNode(*childNodes[nearSideIndex], intersectionInfo);

		bool intersectionFound = IntersectKdTreeNode(*childNodes[nearSideIndex], intersectionInfo);
		if (intersectionFound)
		{
			vector4 point;
			vector4_addScaledVector(intersectionInfo.m_Ray.getPosition(), intersectionInfo.m_Ray.getDirection(), intersectionInfo.m_T, point);

			auto oldT = intersectionInfo.m_T;
			auto oldU = intersectionInfo.m_U;
			auto oldV = intersectionInfo.m_V;
			auto oldIndex = intersectionInfo.m_Index;

			if (pointInPositivePlaneHalf != MathLib::pointOnPositivePlaneSide(node.GetSplittingPlane(), point))
			{
				if (IntersectKdTreeNode(*childNodes[farSideIndex], intersectionInfo))
				{
					if (oldT < intersectionInfo.m_T)
					{
						intersectionInfo.m_T = oldT;
						intersectionInfo.m_U = oldU;
						intersectionInfo.m_V = oldV;
						intersectionInfo.m_Index = oldIndex;
					}
				}
			}
			
			return true;
		}
		
		return IntersectKdTreeNode(*childNodes[farSideIndex], intersectionInfo);
	} 

	bool KdTreeStackTraversal::Traverse(const StaticMeshKdTree& mesh, const ray& intersectionRay, unsigned int* index, float* t, float* u, float* v)
	{
		auto rootNode = mesh.GetRootNode();
		if (nullptr == rootNode)
			return false;

		IntersectionInfo intersectionInfo =
		{
			mesh,
			intersectionRay,
			*index,
			*t,
			*u,
			*v
		};

		return IntersectKdTreeNode(*rootNode, intersectionInfo);
	}
}