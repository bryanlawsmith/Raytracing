#pragma once

#include "Basis.h"
#include "BoundedTraceable.h"
#include <string>

using std::string;
using MathLib::ray;
using MathLib::matrix4x4;

namespace Raytracer
{
	/// <summary>
	/// An instance of a traceable object in the scene.
	/// Also contains a world-space bounding volume and is thus capable 
	/// of being used in conjunction with scene level spatial partitioning.
	/// </summary>
	class GeometryInstance : public BoundedTraceable
	{
	public:

		GeometryInstance(const BoundedTraceable& geometry, const Basis& basis);
		GeometryInstance(const BoundedTraceable& geometry, const vector4& position,
			const quaternion& orientation, const vector4& scale);

		/// <summary>Return the world-space configuration of this geometry instance.</summary>
		Basis& GetBasis();

		/// <summary>Return the world-space configuration of this geometry instance.</summary>
		const Basis& GetBasis() const;

		/// <summary>Reposition this geometry instance in world-space.</summary>
		void SetBasis(const Basis& basis);

		/// BoundedTraceable implementation begin.

		virtual bool Trace(const ray& intersectionRay, float* t, float* results) const override;

		/// BoundedTraceable implementation end.

	private:

		/// <summary>World-space configuration of this geometry instance.</summary>
		Basis m_Basis;

		/// <summary>
		/// The geoemtry data of this geometry instance.
		/// </summary>
		const BoundedTraceable& m_Geometry;

		/// <summary>Used to transform from world-space to this instance's object-space.</summary>
		matrix4x4 m_WorldToObject;

		/// <summary>Used to transform from this instance's object-space to world-space.</summary>
		matrix4x4 m_ObjectToWorld;

		/// <summary>
		/// Recalculates internal fields when the geometry instance is repositioned.
		/// </summary>
		void Update();
	};
}