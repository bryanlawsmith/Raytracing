#ifndef MATHLIB_PLANEOPS_H_INCLUDED
#define MATHLIB_PLANEOPS_H_INCLUDED

#include "MathLibCommon.h"
#include "MathLib_Ray.h"
#include "MathLib_Plane.h"

namespace MathLib
{

/// <summary>
/// If possible calculates the required t value used in generating the intersection point between the plane and
/// the ray. If possible, true is returned and t holds the required value. If not possible, false is returned and
/// t is undefined.
/// </summary>
MATHLIB_INLINE bool intersectRayWithPlane(const ray& ray, const plane& plane, float* t)
{
	float v_dot_n = vector4_dotProduct(ray.getDirection(), plane.getNormal());

	// Determine if the plane and ray direction are parallel.
	if (0.0f == v_dot_n)
		return false;

	vector4 p_sub_r;	// plane point-on-plane (p) sub ray position (r).
	vector4_sub(plane.getPointOnPlane(), ray.getPosition(), p_sub_r);

	// Calculate result.
	*t = vector4_dotProduct(plane.getNormal(), p_sub_r) / v_dot_n;

	return true;
}

MATHLIB_INLINE bool pointOnPositivePlaneSide(const plane& plane, const vector4& point)
{
	vector4 planeToPoint;
	vector4_sub(point, plane.getPointOnPlane(), planeToPoint);

	return (vector4_dotProduct(planeToPoint, plane.getNormal()) >= 0.0f);
}

MATHLIB_INLINE float distanceToPlane(const plane& plane, const vector4& point)
{
	vector4 planeToPoint;
	vector4_sub(point, plane.getPointOnPlane(), planeToPoint);

	return vector4_dotProduct(planeToPoint, plane.getNormal());
}

MATHLIB_INLINE bool sphereInsidePlane(const vector4& spherePosition, float sphereRadius, const plane& plane)
{
	return -distanceToPlane(plane, spherePosition) > sphereRadius;
}

MATHLIB_INLINE bool sphereOutsidePlane(const vector4& spherePosition, float sphereRadius, const plane& plane)
{
	return distanceToPlane(plane, spherePosition) > sphereRadius;
}

MATHLIB_INLINE bool sphereIntersectsPlane(const vector4& spherePosition, float sphereRadius, const plane& plane)
{
	return fabs(distanceToPlane(plane, spherePosition)) <= sphereRadius;
}

}

#endif // MATHLIB_PLANEOPS_H_INCLUDED
