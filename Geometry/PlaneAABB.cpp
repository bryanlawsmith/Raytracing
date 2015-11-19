#include "PlaneAABB.h"

namespace GeometryLib
{
	bool AABBIntersectsPlane(const vector4& min, const vector4& max, const plane& intersectionPlane)
	{
		// Calculate center and half widths.
		vector4 center;
		vector4_add(min, max, center);
		vector4_scale(center, 0.5f, center);

		vector4 halfWidths;
		vector4_sub(max, min, halfWidths);
		vector4_scale(halfWidths, 0.5f, halfWidths);

		return AABBIntersectsPlaneHalfWidths(center, halfWidths, intersectionPlane);
	}

	bool AABBIntersectsPlaneHalfWidths(const vector4& aabbCenter, const vector4& halfWidths, const plane& intersectionPlane)
	{
		const vector4& planeNormal = intersectionPlane.getNormal();
		const float centerX = aabbCenter.extractX();
		const float centerY = aabbCenter.extractY();
		const float centerZ = aabbCenter.extractZ();

		const float halfWidthX = halfWidths.extractX();
		const float halfWidthY = halfWidths.extractY();
		const float halfWidthZ = halfWidths.extractZ();

		vector4 min;
		vector4 max;

		float maxX = centerX + halfWidthX;
		float minX = centerX - halfWidthX;
		if (planeNormal.extractX() > 0.0f)
		{
			max.setX(maxX);
			min.setX(minX);
		}
		else
		{
			max.setX(minX);
			min.setX(maxX);
		}

		float maxY = centerY + halfWidthY;
		float minY = centerY - halfWidthY;
		if (planeNormal.extractY() > 0.0f)
		{
			max.setY(maxY);
			min.setY(minY);
		}
		else
		{
			max.setY(minY);
			min.setY(maxY);
		}

		float maxZ = centerZ + halfWidthZ;
		float minZ = centerZ - halfWidthZ;
		if (planeNormal.extractZ() > 0.0f)
		{
			max.setZ(maxZ);
			min.setZ(minZ);
		}
		else
		{
			max.setZ(minZ);
			min.setZ(maxZ);
		}
		
		max.setW(1.0f);
		min.setW(1.0f);

		if (MathLib::pointOnPositivePlaneSide(intersectionPlane, min) || !MathLib::pointOnPositivePlaneSide(intersectionPlane, max))
			return false;

		return true;
	}
}