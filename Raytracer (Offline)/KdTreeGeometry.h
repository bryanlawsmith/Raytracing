#pragma once

#include "BoundedTraceable.h"
#include <Triangle.h>
#include <StaticMesh.h>

using Assets::StaticMesh;
using GeometryLib::Triangle;

namespace Raytracer
{
	namespace KdTreeConstruction
	{
		class NaiveSpatialMedian;
		class SAH;
	}

	class KdTreeNode;

	/// <summary>
	/// Stores the triangles of a static mesh in a KdTree representation.
	/// </summary>
	class KdTreeGeometry : public BoundedTraceable
	{
	public:

		KdTreeGeometry(const StaticMesh& mesh);
		~KdTreeGeometry();

		KdTreeGeometry& operator=(const KdTreeGeometry& rvalue) = delete;
		KdTreeGeometry& operator=(KdTreeGeometry&& rvalue) = delete;

		/// <summary>
		/// Performs all initialization required for this static mesh.
		/// This includes construction of the kd tree from the available mesh data.
		/// </summary>
		void Initialize(const StaticMesh& mesh);

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

		// Friend class declarations. 
		// TODO: Is there a cleaner way to do this?
		friend class KdTreeConstruction::NaiveSpatialMedian;
		friend class KdTreeConstruction::SAH;
	};
}