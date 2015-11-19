#include "KdTreeConstruction.h"
#include "KdTreeNode.h"
#include "StaticMeshkdTree.h"
#include <Geometry.h>
#include <cassert>
#include <vector>

using namespace std;
using namespace GeometryLib;
using namespace MathLib;

namespace Raytracer
{
	void KdTreeConstruction::ConstructUsingNaiveSpatialMedian(StaticMeshKdTree& mesh)
	{
		auto naiveSpatialMedian = new NaiveSpatialMedian;
		naiveSpatialMedian->BuildKdTree(mesh);
	}

	NaiveSpatialMedian::NaiveSpatialMedian()
	{
	}

	void NaiveSpatialMedian::BuildKdTree(StaticMeshKdTree& mesh)
	{
		m_NumTriangles = mesh.GetNumTriangles();
		m_Triangles = mesh.GetTriangles();

		auto rootNode = new KdTreeNode;

		vector4 boundingMin;
		vector4 boundingMax;
		GeometryLib::ComputeAABBForTriangles(m_NumTriangles, m_Triangles, boundingMin, boundingMax);

		vector4_copy(rootNode->m_BoundingMin, boundingMin);
		vector4_copy(rootNode->m_BoundingMax, boundingMax);

		RecursiveBuild(rootNode, 0);

		mesh.ResetKdTree();
		mesh.m_RootNode = rootNode;
	}

	bool NaiveSpatialMedian::Terminate(unsigned int level)
	{
		return level >= 8;
	}

	void NaiveSpatialMedian::RecursiveBuild(KdTreeNode* node, unsigned int level)
	{
		auto axis = level % 3;

		if (Terminate(level))
		{
			// Then this is a leaf node, determine which triangles intersect this node and store in an index list.
			AssignTriangleListToNode(node);
			return;
		}

		// If termination condition is not met then we need to subdivide this node's space and create another level of the kd tree.
		const vector4& boundingMin = node->GetBoundingMin();
		const vector4& boundingMax = node->GetBoundingMax();

		vector4 boundingMid;
		vector4_add(boundingMin, boundingMax, boundingMid);
		vector4_scale(boundingMid, 0.5f, boundingMid);

		vector4 leftBoundingMin;
		vector4 leftBoundingMax;
		vector4 rightBoundingMin;
		vector4 rightBoundingMax;
		plane splittingPlane;

		vector4_copy(leftBoundingMin, boundingMin);
		vector4_copy(leftBoundingMax, boundingMax);

		vector4_copy(rightBoundingMin, boundingMin);
		vector4_copy(rightBoundingMax, boundingMax);

		float midX = boundingMid.extractX();
		float midY = boundingMid.extractY();
		float midZ = boundingMid.extractZ();

		switch (axis)
		{
		case 0:
			leftBoundingMax.setX(midX);
			rightBoundingMin.setX(midX);
			splittingPlane.setNormal(vector4(1.0f, 0.0f, 0.0f, 0.0f));
			break;

		case 1:
			leftBoundingMax.setY(midY);
			rightBoundingMin.setY(midY);
			splittingPlane.setNormal(vector4(0.0f, 1.0f, 0.0f, 0.0f));
			break;

		case 2:
			leftBoundingMax.setZ(midZ);
			rightBoundingMin.setZ(midZ);
			splittingPlane.setNormal(vector4(0.0f, 0.0f, 1.0f, 0.0f));
			break;
		}

		splittingPlane.setPointOnPlane(boundingMid);
		plane_copy(node->m_SplittingPlane, splittingPlane);

		node->m_Children[0] = new KdTreeNode;
		vector4_copy(node->m_Children[0]->m_BoundingMin, leftBoundingMin);
		vector4_copy(node->m_Children[0]->m_BoundingMax, leftBoundingMax);

		node->m_Children[1] = new KdTreeNode;
		vector4_copy(node->m_Children[1]->m_BoundingMin, rightBoundingMin);
		vector4_copy(node->m_Children[1]->m_BoundingMax, rightBoundingMax);

		RecursiveBuild(node->m_Children[0], level + 1);
		RecursiveBuild(node->m_Children[1], level + 1);
	}

	void NaiveSpatialMedian::AssignTriangleListToNode(KdTreeNode* node)
	{
		// Calculate node center and half widths.
		const vector4& nodeMin = node->GetBoundingMin();
		const vector4& nodeMax = node->GetBoundingMax();

		vector<unsigned int> indexVector;

		for (unsigned int i = 0; i < m_NumTriangles; i++)
			if (TriangleIntersectsAABB(m_Triangles[i], nodeMin, nodeMax))
				indexVector.push_back(i);

		// Copy triangle indices to node.
		size_t numIndices = indexVector.size();
		node->m_NumTriangles = (unsigned int)numIndices;
		node->m_TriangleList = new unsigned int[numIndices];

		for (size_t i = 0; i < numIndices; i++)
			node->m_TriangleList[i] = indexVector[i];
	}
}