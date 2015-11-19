#pragma once

#include "ITraceable.h"
#include <string>
#include <Triangle.h>

using namespace std;
using namespace GeometryLib;

namespace Raytracer
{
	/// <summary>
	/// Stores the triangles of a static mesh instance as a flat list with no higher order data structure.
	/// </summary>
	class StaticMeshNaive : public ITraceable
	{
	public:

		StaticMeshNaive();
		StaticMeshNaive(const string& meshId);
		~StaticMeshNaive();

		StaticMeshNaive& operator=(const StaticMeshNaive& rvalue) = delete;
		StaticMeshNaive& operator=(StaticMeshNaive&& rvalue) = delete;

		void Initialize(const string& meshId);

		Triangle const * GetTriangles() const;

		unsigned int GetNumTriangles() const;

		/// ITraceable implementation begin.

		virtual bool Trace(const ray& intersectionRay, float* t, float* results) const override;

		/// ITraceable implementation end.

	protected:

		unsigned int m_NumTriangles;

		Triangle* m_Triangles;

		void FreeMemory();
	};
}