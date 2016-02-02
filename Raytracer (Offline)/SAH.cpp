#include "SAH.h"
#include "KdTreeGeometry.h"
#include "KdTreeNode.h"
#include <vector>
#include <Geometry.h>
#include <cassert>
#include <algorithm>

using namespace std;
using namespace MathLib;
using namespace GeometryLib;

namespace Raytracer
{
namespace KdTreeConstruction
{
	SAH::SAH(unsigned maxDepth, unsigned maxTriangles) : 
		m_MaxDepth(maxDepth), 
		m_MaxTriangles(maxTriangles)
	{
		for (unsigned i = 0; i < 3; i++)
			m_Edges[i] = nullptr;
	}

	SAH::~SAH()
	{
		for (unsigned i = 0; i < 3; i++)
			if (m_Edges[i]) delete[] m_Edges[i];
	}

	void SAH::Construct(KdTreeGeometry& geometry)
	{
		m_NumTriangles = geometry.GetNumTriangles();
		m_Triangles = geometry.GetTriangles();

		// Create initial indices vector.
		vector<unsigned> triangleIndices;
		for (unsigned i = 0; i < m_NumTriangles; i++)
			triangleIndices.push_back(i);

		// Calculate triangle bounds.
		m_TriangleBounds.clear();
		m_TriangleBounds.reserve(m_NumTriangles);
		for (unsigned i = 0; i < m_NumTriangles; i++)
		{
			m_TriangleBounds.push_back(BoundingBox());
			m_Triangles[i].ComputeBounds(m_TriangleBounds[i].m_Min, m_TriangleBounds[i].m_Max);
		}

		for (unsigned i = 0; i < 3; i++)
			m_Edges[i] = new BoundEdge[m_NumTriangles * 2];

		auto rootNode = new KdTreeNode;
		CalculateBoundingVolume(rootNode->m_BoundingMin, rootNode->m_BoundingMax,
			triangleIndices);

		EvaluateSAH(*rootNode, triangleIndices, 0, 0);

		geometry.ResetKdTree();
		geometry.m_RootNode = rootNode;
	}

	float SAH::CalculateBoundingBoxSurfaceArea(const vector4& min, const vector4& max)
	{
		float width = max.x - min.x;
		float height = max.y - min.y;
		float length = max.z - min.z;

		float areaWidthHeight = width * height;
		float areaWidthLength = width * length;
		float areaHeightLength = height * length;

		return 2.0f * (areaWidthHeight + areaWidthLength + areaHeightLength);
	}

	struct Bucket
	{
		Bucket() { m_Count = 0; }

		unsigned m_Count;

		vector4 m_Min;
		vector4 m_Max;
	};

	static int DetermineGreatestExtent(const vector4& min, const vector4& max)
	{
		vector4 extents;
		vector4_sub(max, min, extents);

		unsigned index = extents[1] > extents[0] ? 1 : 0;
		index = extents[2] > extents[index] ? 2 : index;
		
		return index;
	}

	void SAH::EvaluateSAH(KdTreeNode& node, vector<unsigned> indices, unsigned depth, 
		int badRefines)
	{
		const float triangleIntersectionCost = 1.0f;
		const float traversalCost = 8.0f;

		unsigned numTriangles = (unsigned)indices.size();

		auto& nodeBoundsMin = node.m_BoundingMin;
		auto& nodeBoundsMax = node.m_BoundingMax;

		if (numTriangles < m_MaxTriangles || depth == m_MaxDepth)
		{
			// Create leaf node.
			InitializeLeafNode(node, indices);
			return;
		}
		assert(depth <= m_MaxDepth);

		// Choose split axis position for interior node.
		int bestAxis = -1;
		int bestOffset = -1;
		float bestCost = FLT_MAX;
		float oldCost = triangleIntersectionCost * (float)numTriangles;
		float surfaceArea = CalculateBoundingBoxSurfaceArea(node.GetBoundingMin(),
			node.GetBoundingMax());
		float surfaceAreaInv = 1.0f / surfaceArea;

		auto splitAxis = DetermineGreatestExtent(node.GetBoundingMin(), 
			node.GetBoundingMax());

		unsigned retries = 0;

	RetrySplit:

		// Initialize edges for axis.
		for (unsigned i = 0; i < numTriangles; i++)
		{
			auto triangleIndex = indices[i];
			
			auto& triangleBoundMin = m_TriangleBounds[triangleIndex].m_Min;
			auto& triangleBoundMax = m_TriangleBounds[triangleIndex].m_Max;
			
			m_Edges[splitAxis][2 * i] = BoundEdge(triangleBoundMin[splitAxis], triangleIndex, true);
			m_Edges[splitAxis][2 * i + 1] = BoundEdge(triangleBoundMax[splitAxis], triangleIndex, false);
		}
		sort(&m_Edges[splitAxis][0], &m_Edges[splitAxis][2 * numTriangles]);

		// Compute the costs for all splits along the selected axis, and attempt to find the best.
		int numBelow = 0;
		int numAbove = numTriangles;
		for (int i = 0; i < 2 * numTriangles; i++)
		{
			auto& currentEdge = m_Edges[splitAxis][i];

			if (currentEdge.m_Type == BoundEdge::END)
				numAbove--;

			if (currentEdge.m_T > nodeBoundsMin[splitAxis] && 
				currentEdge.m_T < nodeBoundsMax[splitAxis])
			{
				// Compute cost for split at ith edge.
				vector4 belowBounds[2];
				vector4_copy(belowBounds[0], nodeBoundsMin);
				vector4_copy(belowBounds[1], nodeBoundsMax);
				belowBounds[1][splitAxis] = currentEdge.m_T;

				vector4 aboveBounds[2];
				vector4_copy(aboveBounds[0], nodeBoundsMin);
				vector4_copy(aboveBounds[1], nodeBoundsMax);
				aboveBounds[0][splitAxis] = currentEdge.m_T;

				float currentSurfaceArea[2];
				currentSurfaceArea[0] = CalculateBoundingBoxSurfaceArea(belowBounds[0], belowBounds[1]);
				currentSurfaceArea[1] = CalculateBoundingBoxSurfaceArea(aboveBounds[0], aboveBounds[1]);

				assert(currentSurfaceArea[0] != 0.0f && currentSurfaceArea[1] != 0.0f);

				currentSurfaceArea[0] *= surfaceAreaInv;
				currentSurfaceArea[1] *= surfaceAreaInv;
					 
				// TODO: Implement the "empty bonus" paradigm as well? 
				float cost = traversalCost + triangleIntersectionCost *
					(numBelow * currentSurfaceArea[0]) + (numAbove * currentSurfaceArea[1]);

				// Update best split position if this is the lowest cost so far.
				if (cost < bestCost)
				{
					bestCost = cost;
					bestAxis = splitAxis;
					// TODO: Remove unsigned ints from usage? 
					bestOffset = i;
				}
			}

			if (currentEdge.m_Type == BoundEdge::START)
				numBelow++;
		}
		assert(numBelow == numTriangles && numAbove == 0);

		// If no suitable splits were found for the initial axis, attempt to find splits along 
		// the other two axes.
		if (bestAxis == -1 && retries < 2)
		{
			retries++;
			splitAxis = (splitAxis + 1) % 3;
			goto RetrySplit;
		}

		// If no good split can be found along any axis for this node, attempt to continue because
		// following nodes may have a good split.
		if (bestCost > oldCost)
			badRefines++;

		// Create leaf if no best splits were found.
		if ((bestCost > 4.0f * oldCost && numTriangles < 16) ||
			bestAxis == -1 || badRefines == 3)
		{
			// It is not worth it at this point to continue splitting. Create a leaf node.
			InitializeLeafNode(node, indices);
			return;
		}

		// Classify primitives with respect to the split.
		vector<unsigned> childIndices[2];
		for (unsigned i = 0; i < bestOffset; i++)
			if (m_Edges[bestAxis][i].m_Type == BoundEdge::START)
				childIndices[0].push_back(m_Edges[bestAxis][i].m_PrimitiveIndex);

		for (unsigned i = bestOffset; i < 2 * numTriangles; i++)
			if (m_Edges[bestAxis][i].m_Type == BoundEdge::END)
				childIndices[1].push_back(m_Edges[bestAxis][i].m_PrimitiveIndex);

		// Recursively initialize child nodes.
		plane splittingPlane;

		vector4 splitNormal;
		splitNormal.setXYZW(0.0f, 0.0f, 0.0f, 0.0f);
		splitNormal[bestAxis] = 1.0f;
		splittingPlane.setNormal(splitNormal);

		vector4 splitPosition;
		vector4_copy(splitPosition, nodeBoundsMin);
		splitPosition[bestAxis] = m_Edges[bestAxis][bestOffset].m_T;
		splittingPlane.setPointOnPlane(splitPosition);
		plane_copy(node.m_SplittingPlane, splittingPlane);
		
		auto childNode0 = new KdTreeNode;
		vector4_copy(childNode0->m_BoundingMin, nodeBoundsMin);
		vector4_copy(childNode0->m_BoundingMax, nodeBoundsMax);
		
		auto childNode1 = new KdTreeNode;
		vector4_copy(childNode1->m_BoundingMin, nodeBoundsMin);
		vector4_copy(childNode1->m_BoundingMax, nodeBoundsMax);

		childNode0->m_BoundingMax[bestAxis] = childNode1->m_BoundingMin[bestAxis] = 
			m_Edges[bestAxis][bestOffset].m_T;

		node.m_Children[0] = childNode0;
		node.m_Children[1] = childNode1;

		EvaluateSAH(*node.m_Children[0], childIndices[0], depth + 1, badRefines);
		EvaluateSAH(*node.m_Children[1], childIndices[1], depth + 1, badRefines);
	}

	void SAH::DetermineIntersectingTriangles(KdTreeNode& node,
		unique_ptr<unsigned[]>& triangleIndices, unsigned numTriangles,
		unique_ptr<unsigned[]>& newTriangleIndices, unsigned& newTriangleCounts)
	{
		vector<unsigned> indexVector;

		const vector4& nodeMin = node.GetBoundingMin();
		const vector4& nodeMax = node.GetBoundingMax();

		for (unsigned i = 0; i < numTriangles; i++)
			if (GeometryLib::TriangleIntersectsAABB(m_Triangles[triangleIndices[i]], 
				nodeMin, nodeMax))
				indexVector.push_back(triangleIndices[i]);

		newTriangleIndices = make_unique<unsigned[]>(indexVector.size());
		newTriangleCounts = (unsigned)indexVector.size();

		for (unsigned i = 0; i < newTriangleCounts; i++)
			newTriangleIndices[i] = indexVector[i];
	}

	void SAH::AssignTriangleListToNode(KdTreeNode& node, 
		unique_ptr<unsigned[]>& triangleIndices, unsigned numTriangles)
	{
		// Copy triangle indices to node.
		node.m_NumTriangles = numTriangles;
		node.m_TriangleList = new unsigned[numTriangles];

		for (unsigned i = 0; i < numTriangles; i++)
			node.m_TriangleList[i] = triangleIndices[i];
	}

	void SAH::CalculateBoundingVolume(vector4& minExtents, vector4& maxExtents, 
		vector<unsigned> indices)
	{
		minExtents.setXYZ(FLT_MAX, FLT_MAX, FLT_MAX);
		maxExtents.setXYZ(-FLT_MAX, -FLT_MAX, -FLT_MAX);

		for (size_t i = 0; i < indices.size(); i++) // Iterate over triangles.
		{
			for (int j = 0; j < 3; j++)	// Iterate over triangle vertices.
			{
				for (int k = 0; k < 3; k++) // Iterate over position elements.
				{
					float currentElement = m_Triangles[indices[i]].m_Vertices[j].m_Position[k];
					minExtents[k] = min(minExtents[k], currentElement);
					maxExtents[k] = max(maxExtents[k], currentElement);
				}
			}
		}
	}

	void SAH::InitializeLeafNode(KdTreeNode& node, vector<unsigned>& indices)
	{
		unsigned indicesCount = (unsigned)indices.size();

		node.m_NumTriangles = indicesCount;
		node.m_TriangleList = new unsigned[indicesCount];

		for (unsigned i = 0; i < indicesCount; i++)
			node.m_TriangleList[i] = indices[i];
	}
}
}