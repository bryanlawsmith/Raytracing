#pragma once

#include "IKdTreeBuilder.h"

namespace GeometryLib
{
	class Triangle;
}

namespace Raytracer
{
	class KdTreeNode;

namespace KdTreeConstruction
{

	/// <summary>
	/// Class responsible for constructing a kd tree using the naive spatial  
	/// median algorithm.
	/// </summary>
	class NaiveSpatialMedian : public IKdTreeBuilder
	{
	public:

		NaiveSpatialMedian();

		void Construct(KdTreeGeometry& geometry) override;

	protected:

		unsigned m_NumTriangles;

		const GeometryLib::Triangle* m_Triangles;
		
		bool Terminate(unsigned level);

		void RecursiveBuild(KdTreeNode* node, unsigned level);

		void AssignTriangleListToNode(KdTreeNode* node);
	};

}
}