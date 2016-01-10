#pragma once

#include <Triangle.h>

using namespace GeometryLib;

namespace Raytracer
{
	class KdTreeGeometry;
	class KdTreeNode;

	class KdTreeConstruction
	{
	public:

		/// <summary>
		/// Constructs the kd tree using the naive spatial median algorithm.
		/// </summary>
		static void ConstructUsingNaiveSpatialMedian(KdTreeGeometry& mesh);
	};

	class NaiveSpatialMedian
	{
	public:

		NaiveSpatialMedian();

		void BuildKdTree(KdTreeGeometry& mesh);

		bool Terminate(unsigned int level);

		void RecursiveBuild(KdTreeNode* node, unsigned int level);

	protected:

		unsigned int m_NumTriangles;

		const Triangle* m_Triangles;

		void AssignTriangleListToNode(KdTreeNode* node);
	};

}