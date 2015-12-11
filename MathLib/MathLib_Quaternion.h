#ifndef MATHLIB_QUATERNION_H_INCLUDED
#define MATHLIB_QUATERNION_H_INCLUDED

#include "MathLibCommon.h"

namespace MathLib
{

class matrix4x4;

class quaternion
{
public:

	union
	{
		struct
		{
			float w, x, y, z;
		} MATHLIB_ALIGN(16);
	};

	quaternion()
	{
	}

	MATHLIB_INLINE float extractX() const { return x; }
	MATHLIB_INLINE float extractY() const { return y; }
	MATHLIB_INLINE float extractZ() const { return z; }
	MATHLIB_INLINE float extractW() const { return w; }

	quaternion(const quaternion& other)
	{
		w = other.w;
		x = other.x;
		y = other.y;
		z = other.z;
	}

	MATHLIB_INLINE void SetWXYZ(float w, float x, float y, float z)
	{
		this->w = w;
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

MATHLIB_INLINE void quaternion_copy(quaternion& dest, const quaternion& source)
{
	dest.w = source.w;
	dest.x = source.x;
	dest.y = source.y;
	dest.z = source.z;
}

MATHLIB_INLINE void quaternion_setToIdentity(quaternion& q)
{
	q.w = 1.0f;
	q.x = 0.0f;
	q.y = 0.0f;
	q.z = 0.0f;
}

MATHLIB_INLINE void quaternion_fromAxisAngle(quaternion& q, float axisX, float axisY, float axisZ, float angle)
{
	float angleRadians = MATHLIB_DEG_TO_RAD(angle * 0.5f);
	float sinAngle = sinf(angleRadians);
	float cosAngle = cosf(angleRadians);

	q.w = cosAngle;
	q.x = sinAngle * axisX;
	q.y = sinAngle * axisY;
	q.z = sinAngle * axisZ;
}

MATHLIB_INLINE float quaternion_magnitude(const quaternion& q)
{
	return sqrtf((q.w * q.w) +
				 (q.x * q.x) +
				 (q.y * q.y) +
				 (q.z * q.z));
}

MATHLIB_INLINE void quaternion_conjugate(const quaternion& q, quaternion& conjugate)
{
	conjugate.w = q.w;
	conjugate.x = -q.x;
	conjugate.y = -q.y;
	conjugate.z = -q.z;
}

MATHLIB_INLINE void quaternion_inverse(const quaternion& q, quaternion& inverse)
{
	float magnitudeRecip = 1.0f / quaternion_magnitude(q);

	inverse.w = q.w * magnitudeRecip;
	inverse.x = -q.x * magnitudeRecip;
	inverse.y = -q.y * magnitudeRecip;
	inverse.z = -q.z * magnitudeRecip;
}

MATHLIB_INLINE void quaternion_product(const quaternion& q1, const quaternion& q2, quaternion& result)
{
	result.w = (q1.w * q2.w) - (q1.x * q2.x) - (q1.y * q2.y) - (q1.z * q2.z);
	result.x = (q1.w * q2.x) + (q1.x * q2.w) + (q1.z * q2.y) - (q1.y * q2.z);
	result.y = (q1.w * q2.y) + (q1.y * q2.w) + (q1.x * q2.z) - (q1.z * q2.x);
	result.z = (q1.w * q2.z) + (q1.z * q2.w) + (q1.y * q2.x) - (q1.x * q2.y);
}

MATHLIB_INLINE void quaternion_normalize(quaternion& q)
{
	float magnitude = quaternion_magnitude(q);

	if (magnitude > 0.0f)
	{
		// Normalize the quaternion.
		float magnitudeRecip = 1.0f / magnitude;
		q.w *= magnitudeRecip;
		q.x *= magnitudeRecip;
		q.y *= magnitudeRecip;
		q.z *= magnitudeRecip;
	}
	else
	{
		quaternion_setToIdentity(q);
	}
}

MATHLIB_INLINE void quaternion_fromEulerAngles(quaternion& q, float rotationX, float rotationY, float rotationZ)
{
	quaternion xAxisRotation;
	quaternion yAxisRotation;
	quaternion zAxisRotation;

	quaternion_fromAxisAngle(xAxisRotation, 1.0f, 0.0f, 0.0f, rotationX);
	quaternion_fromAxisAngle(yAxisRotation, 0.0f, 1.0f, 0.0f, rotationY);
	quaternion_fromAxisAngle(zAxisRotation, 0.0f, 0.0f, 1.0f, rotationZ);

	quaternion yxRotation;
	quaternion_product(yAxisRotation, xAxisRotation, yxRotation);
	quaternion_product(zAxisRotation, yxRotation, q);
}

}


#endif // MATHLIB_QUATERNION_H_INCLUDED
