#pragma once

#include "IKdTreeBuilder.h"
#include <MathLib.h>
#include <memory>
#include <vector>

using std::shared_ptr;
using std::unique_ptr;
using std::vector;
using MathLib::vector4;

namespace GeometryLib
{
	class Triangle;
}
using GeometryLib::Triangle;

namespace Raytracer
{
	class KdTreeNode;

namespace KdTreeConstruction
{
	/// <summary>
	/// Class responsible for constructing a kd tree using the Surface
	/// Area Heuristic algorithm.
	/// </summary>
	class SAH : public IKdTreeBuilder
	{
	public:

		SAH(unsigned maxDepth, unsigned maxTriangles);
		~SAH();

		void Construct(KdTreeGeometry& geometry) override;
		
	protected:

		struct BoundingBox
		{
			vector4 m_Min;
			vector4 m_Max;
		};
		vector<BoundingBox> m_TriangleBounds;

		struct BoundEdge
		{
			float m_T;
			
			unsigned m_PrimitiveIndex;
			
			enum { START, END } m_Type;

			BoundEdge() {}
			BoundEdge(float t, int primitiveIndex, bool starting) :
				m_T(t), m_PrimitiveIndex(primitiveIndex), m_Type(starting ? START : END)
			{}

			bool operator<(const BoundEdge& edge) const
			{
				if (m_T == edge.m_T)
					return (int)m_Type < (int)edge.m_Type;
				else
					return m_T < edge.m_T;
			}
		};
		BoundEdge* m_Edges[3];

		unsigned m_NumTriangles;
		const Triangle* m_Triangles;

		unsigned m_MaxDepth;
		unsigned m_MaxTriangles;

		float CalculateBoundingBoxSurfaceArea(const vector4& min, const vector4& max);

		/// <summary>Perform SAH evaluation on this node.</summary>
		void EvaluateSAH(KdTreeNode& node, vector<unsigned> indices, unsigned depth,
			int badRefines);

		/// <summary>
		/// Determines the subset of the provided array which intersect the KdTreeNode.
		/// Allocates and returns memory for the subset array as a shared_ptr.
		/// </summary>
		void DetermineIntersectingTriangles(KdTreeNode& node,
			unique_ptr<unsigned[]>& triangleIndices, unsigned numTriangles,
			unique_ptr<unsigned[]>& newTriangleIndices, unsigned& newTriangleCounts);

		/// <summary>Copies the triangle index array into the node.</summary>
		void AssignTriangleListToNode(KdTreeNode& node, 
			unique_ptr<unsigned[]>& triangleIndices, unsigned numTriangles);

		void CalculateBoundingVolume(vector4& minExtents, vector4& maxExtents, 
			vector<unsigned> indices);

		void InitializeLeafNode(KdTreeNode& node, vector<unsigned>& triangleIndices);
	};
}
}