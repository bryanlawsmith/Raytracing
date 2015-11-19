#pragma once

#include "ITraceable.h"
#include <string>
#include <Triangle.h>

using namespace std;
using namespace GeometryLib;

namespace Raytracer
{
	class KdTreeNode;

	class StaticMeshKdTree : public ITraceable
	{
	public:

		StaticMeshKdTree();
		StaticMeshKdTree(const string& meshId);
		~StaticMeshKdTree();

		StaticMeshKdTree& operator=(const StaticMeshKdTree& rvalue) = delete;
		StaticMeshKdTree& operator=(StaticMeshKdTree&& rvalue) = delete;

		/// <summary>
		/// Performs all initialization required for this static mesh.
		/// This includes construction of the kd tree from the available mesh data.
		/// </summary>
		void Initialize(const string& meshId);

		Triangle const * GetTriangles() const;

		unsigned int GetNumTriangles() const;

		/// ITraceable implementation begin.

		virtual bool Trace(const ray& intersectionRay, float* t, float* results) const override;

		/// ITraceable implementation end.

		/// <summary>
		/// Returns the root node of the kd tree.
		/// </summary>
		KdTreeNode* GetRootNode() const;

	protected:

		unsigned int m_NumTriangles;

		Triangle* m_Triangles;

		KdTreeNode* m_RootNode;

		/// </summary>
		/// Frees all resources allocated for this instance.
		/// </summary>
		void FreeMemory();

		/// <summary>
		/// Resets just the of the kd tree nodes. Useful when needing to rebuild the kd tree.
		/// </summary>
		void ResetKdTree();

		friend class KdTreeConstruction;
		friend class NaiveSpatialMedian;
	};
}