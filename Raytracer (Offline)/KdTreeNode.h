#pragma once

#include <MathLib.h>

using namespace MathLib;

namespace Raytracer
{
	namespace KdTreeConstruction
	{
		class NaiveSpatialMedian;
		class SAH;
	}

	class KdTreeNode
	{
	public:

		/// <summary>
		/// Constructs this KdTreeNode instance.
		/// </summary>
		KdTreeNode();

		/// <summary>
		/// Destructs this KdTreeNode instance.
		/// </summary>
		~KdTreeNode();

		/// <summary>
		/// Returns the minimal extents of this KdTreeNode's voxel.
		/// </summary>
		const vector4& GetBoundingMin() const;

		/// <summary>
		/// Returns the maximal extents of this KdTreeNode's voxel.
		/// </summary>
		const vector4& GetBoundingMax() const;
		
		/// <summary>
		/// Returns the splitting plane of this KdTreeNode.
		/// </summary>
		const plane& GetSplittingPlane() const;

		/// <summary>
		/// Returns the children of this node.
		/// </summary>
		KdTreeNode* const * GetChildren() const;

		/// <summary>
		/// Determines whether or not this KdTreeNode is a child node.
		/// </summary>
		bool IsChild() const;

		/// <summary>
		/// Returns the number of triangles contained in this KdTree node.
		/// </summary>
		unsigned int GetNumTriangles() const;

		/// <summary>
		/// Returns the indices of the triangles contained in this node.
		/// <summary>
		const unsigned int * GetTriangleList() const;

	protected:

		KdTreeNode(const KdTreeNode& rvalue) = delete;
		KdTreeNode& operator=(const KdTreeNode& rvalue) = delete;

		vector4 m_BoundingMin;
		vector4 m_BoundingMax;

		unsigned int* m_TriangleList;
		unsigned int m_NumTriangles;

		plane m_SplittingPlane;

		KdTreeNode* m_Children[2];

		// Friend class declarations. 
		// TODO: Is there a cleaner way to do this?
		friend class Raytracer::KdTreeConstruction::NaiveSpatialMedian;
		friend class Raytracer::KdTreeConstruction::SAH;
	};
}