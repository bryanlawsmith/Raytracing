#pragma once

#include "GeometryLibCommon.h"
#include <MathLib.h>

using namespace MathLib;

namespace GeometryLib
{
	class Triangle;

	/// <summary>
	/// Determines whether a triangle intersects an AABB.
	/// </summary>
	/// <param name="triangle">The triangle to intersect with the AABB.</param>
	/// <param name="min">The minimum bounding extents of the AABB.</param>
	/// <param name="max">The maximum bounding extents of the AABB.</param>
	/// <returns>True if there exists an intersection, false if not.</returns>
	bool TriangleIntersectsAABB(const Triangle& triangle, const vector4& min, const vector4& max);

	/// <summary>
	/// Determines the minimal and maximal extents of an AABB volume enclosing the specified list of triangles.
	/// </summary>
	/// <param name="numTriangles">The number of triangles in the specified triangle list.</param>
	/// <param name="triangles">Array of triangles to process.</param>
	/// <param name="min">Reference to the vector that will store the minimal values.</pararm>
	/// <param name="max">Reference to the vector that will store the maximal values.</param>
	void ComputeAABBForTriangles(unsigned int numTriangles, const Triangle* triangles, vector4& min, vector4& max);
}