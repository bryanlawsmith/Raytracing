#pragma once 

#include <MathLib.h>
#include "GeometryLibCommon.h"

using namespace MathLib;

namespace GeometryLib
{
	/// <summary>
	/// Determines whether the AABB intersects the plane (some vertices on one side of the plane and others on the 
	/// other side of the plane).
	/// </summary>
	/// <param name="min">The minimal bounding extents of the AABB.</param>
	/// <param name="max">The maximal bounding extents of the AABB.</param>
	/// <param name="intersectionPlane">The plane to intersect the AABB against.</param>
	/// <returns>
	/// True if there is an intersection, false if not. 
	/// </returns>
	bool AABBIntersectsPlane(const vector4& min, const vector4& max, const plane& intersectionPlane);

	/// <summary>
	/// Determines whether the AABB intersects the plane (some vertices on one side of the plane and others on the 
	/// other side of the plane).
	/// </summary>
	/// <param name="aabbCenter">The center of the AABB.</param>
	/// <param name="halfWidths">The half extents of the AABB.</param>
	/// <param name="intersectionPlane">The plane to intersect the AABB against.</param>
	/// <returns>
	/// True if there is an intersection, false if not. 
	/// </returns>
	bool AABBIntersectsPlaneHalfWidths(const vector4& aabbCenter, const vector4& halfWidths, const plane& intersectionPlane);
}