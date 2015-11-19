#pragma once

#include <MathLib.h>

using namespace MathLib;

namespace GeometryLib
{
	/// <summary>
	/// Intersects a ray with an AABB.
	/// </summary>
	/// <param name="intersectionRay">The ray to intersect with the triangle.</param>
	/// <param name="tEntry">If successful, will return the t value of the entry point of the ray with the AABB.</param>
	/// <param name="tExit">If successful, will return the t value of the exit point of the ray with the AABB.</param>
	/// <param name="aabbVertices">Holds the minimal and maximal extents of the AABB.</param>
	/// <returns>
	/// Returns true if there exists an intersection, false if not.
	/// </returns>
	bool RayIntersectsAABB(const ray& intersectionRay, float* tEntry, float* tExit, const vector4 * aabbVertices);

	/// <summary>
	/// Intersects a ray with an AABB.
	/// </summary>
	/// <param name="intersectionRay">The ray to intersect with the triangle.</param>
	/// <param name="tEntry">If successful, will return the t value of the entry point of the ray with the AABB.</param>
	/// <param name="tExit">If successful, will return the t value of the exit point of the ray with the AABB.</param>
	/// <param name="aabbMinExtents">Holds the minimal extents of the AABB.</param>
	/// <param name="aabbMaxExtents">Holds the maximal extents of the AABB.</param>
	/// <returns>
	/// Returns true if there exists an intersection, false if not.
	/// </returns>
	bool RayIntersectsAABB(const ray& intersectionRay, float* tEntry, float* tExit, const vector4& aabbMinExtents, const vector4& aabbMaxExtents);
}
