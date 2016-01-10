#pragma once

#include "ITraceable.h"
#include <MathLib.h>

namespace Raytracer
{
	// This class defines a traceable object that has an AABB computed for it.
	class BoundedTraceable : public ITraceable
	{
	public:

		virtual ~BoundedTraceable();

		/// ITraceable implementation begin.
		virtual bool Trace(const ray& intersectionRay, float* t, float* results) const = 0;
		/// ITraceable implementation end.

		/// <summary>Return an immutable pointer to the min/max extents of the bounding volume.</summary>
		const MathLib::vector4* GetBounds() const
		{
			return m_Bounds;
		}

	protected:

		/// <summary>Minimum and maximum extents.</summary>
		MathLib::vector4 m_Bounds[2];
	};
}