#pragma once

#include <MathLib.h>
#include "Triangle.h"

using namespace MathLib;

namespace GeometryLib
{
	/// <summary>
	/// Intersects a ray with a given triangle.
	/// </summary>
	/// <param name="intersectionRay">The ray to intersect with the triangle.</param>
	/// <param name="triangle">The triangle to intersect against.</param>
	/// <param name="t">If successful, will contain the t value to generate the intersection point.</param>
	/// <param name="u">If successful, will contain the barycentric u coordinate at the intersection point.</param>
	/// <param name="v">If successful, will contain the barycentric v coordinate at the intersection point.</param>
	/// <returns>True if there exists an intersection, false if not.</returns>
	bool RayTriangleIntersection(const ray& intersectionRay, 
								 const Triangle& triangle, 
								 float& t, 
								 float& u,
								 float& v);
}