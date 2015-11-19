#pragma once

#include <Triangle.h>

using namespace GeometryLib;

namespace Raytracer
{
	class StaticMeshKdTree;
	class KdTreeNode;

	class KdTreeConstruction
	{
	public:

		/// <summary>
		/// Constructs the kd tree using the naive spatial median algorithm.
		/// </summary>
		static void ConstructUsingNaiveSpatialMedian(StaticMeshKdTree& mesh);
	};

	class NaiveSpatialMedian
	{
	public:

		NaiveSpatialMedian();

		void BuildKdTree(StaticMeshKdTree& mesh);

		bool Terminate(unsigned int level);

		void RecursiveBuild(KdTreeNode* node, unsigned int level);

	protected:

		unsigned int m_NumTriangles;

		const Triangle* m_Triangles;

		void AssignTriangleListToNode(KdTreeNode* node);
	};

}