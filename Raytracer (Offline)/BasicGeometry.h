#pragma once

#include "BoundedTraceable.h"
#include <StaticMesh.h>
#include <Triangle.h>

using GeometryLib::Triangle;
using Assets::StaticMesh;

namespace Raytracer
{
	/// <summary>
	/// Stores the triangles of a static mesh as a flat list with no spatial partitioning
	/// data structure.
	/// </summary>
	class BasicGeometry : public BoundedTraceable
	{
	public:

		BasicGeometry(const StaticMesh& mesh);
		
		~BasicGeometry();

		BasicGeometry& operator=(const BasicGeometry& rvalue) = delete;
		BasicGeometry& operator=(BasicGeometry&& rvalue) = delete;

		/// <summary>
		/// Generates the internal data from the provided StaticMesh.
		/// </summary>
		void Initialize(const StaticMesh& mesh);

		Triangle const * GetTriangles() const;

		unsigned GetNumTriangles() const;

		/// ITraceable implementation begin.

		bool Trace(const ray& intersectionRay, float* t, float* results) const override;

		/// ITraceable implementation end.

	protected:

		Triangle* m_Triangles;

		unsigned int m_NumTriangles;

		/// <summary>
		/// Releases any allocated internal data.
		/// </summary>
		void FreeMemory();
	};
}