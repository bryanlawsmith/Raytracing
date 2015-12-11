#ifndef MATHLIB_PLANE_H_INCLUDED
#define MATHLIB_PLANE_H_INCLUDED

#include "MathLib_Vector4.h"

namespace MathLib
{

class plane
{
public:

	vector4 pointOnPlane;
	vector4 normal;

	plane()
	{
	}

	plane(const vector4& pointOnPlane, const vector4& normal)
	{
		MathLib::vector4_copy(plane::pointOnPlane, pointOnPlane);
		MathLib::vector4_copy(plane::normal, normal);
	}

	plane(float pointOnPlane_x, float pointOnPlane_y, float pointOnPlane_z,
			float normal_x, float normal_y, float normal_z)
	{
		pointOnPlane.setXYZW(pointOnPlane_x, pointOnPlane_y, pointOnPlane_z, 1.0f);
		normal.setXYZW(normal_x, normal_y, normal_z, 0.0f);
		MathLib::vector4_normalize(normal);
	}

	plane(const plane& other)
	{
		MathLib::vector4_copy(plane::pointOnPlane, other.pointOnPlane);
		MathLib::vector4_copy(plane::normal, other.normal);
	}

	MATHLIB_INLINE void setPointOnPlane(const vector4& pointOnPlane)
	{
		MathLib::vector4_copy(plane::pointOnPlane, pointOnPlane);
	}

	MATHLIB_INLINE void setNormal(const vector4& normal)
	{
		MathLib::vector4_copy(plane::normal, normal);
		MathLib::vector4_normalize(plane::normal);
		MathLib::vector4_setToVector(plane::normal);
	}

	MATHLIB_INLINE void setNormal(float x, float y, float z)
	{
		plane::normal.setXYZW(x, y, z, 0.0f);
		MathLib::vector4_normalize(plane::normal);
		MathLib::vector4_setToVector(plane::normal);
	}

	MATHLIB_INLINE const vector4& getPointOnPlane() const
	{
		return pointOnPlane;
	}

	MATHLIB_INLINE const vector4& getNormal() const
	{
		return normal;
	}
} MATHLIB_ALIGN(16);

MATHLIB_INLINE void plane_copy(plane& dest, const plane& src)
{
	vector4_copy(dest.pointOnPlane, src.pointOnPlane);
	vector4_copy(dest.normal, src.normal);
}

}

#endif // MATHLIB_PLANE_H_INCLUDED
