#include "RayAABB.h"
#include <CommonDefines.h>

namespace GeometryLib
{
	bool RayIntersectsAABB(const ray& intersectionRay, float* tEntry, float* tExit, const vector4 * aabbVertices)
	{
		return RayIntersectsAABB(intersectionRay, tEntry, tExit, aabbVertices[AABB_EXTENTS_MIN], aabbVertices[AABB_EXTENTS_MAX]);
	}

	bool RayIntersectsAABB(const ray& intersectionRay, float* tEntry, float* tExit, const vector4& aabbMinExtents, const vector4& aabbMaxExtents)
	{
		// Test against front plane.
		const vector4& rayPosition = intersectionRay.getPosition();
		const vector4& rayDirection = intersectionRay.getDirection();

		bool tFound = false;
		float minimumTValue = FLT_MAX;
		float maximumTValue = 0.0f;

		// Containment test. Is the ray inside the aabb?
		if (rayPosition.extractX() >= aabbMinExtents.extractX() &&
			rayPosition.extractX() <= aabbMaxExtents.extractX() &&
			rayPosition.extractY() >= aabbMinExtents.extractY() &&
			rayPosition.extractY() <= aabbMaxExtents.extractY() &&
			rayPosition.extractZ() >= aabbMinExtents.extractZ() &&
			rayPosition.extractZ() <= aabbMaxExtents.extractZ())
		{
			minimumTValue = 0.0f;

			tFound = true;
		}

		// Intersect ray with the minimum x plane of the aabb.
		plane minXPlane(vector4(aabbMinExtents.extractX(), 0.0f, 0.0f, 1.0f), vector4(-1.0f, 0.0f, 0.0f, 0.0f));

		float minXPlaneIntersection;
		if (MathLib::intersectRayWithPlane(intersectionRay, minXPlane, &minXPlaneIntersection))
		{
			// Construct the point on the plane. Test if in bounds of the aabb.
			vector4 point;
			vector4_addScaledVector(rayPosition, rayDirection, minXPlaneIntersection, point);

			if (point.extractY() >= aabbMinExtents.extractY() &&
				point.extractY() <= aabbMaxExtents.extractY() &&
				point.extractZ() >= aabbMinExtents.extractZ() &&
				point.extractZ() <= aabbMaxExtents.extractZ())
			{
				if (minXPlaneIntersection >= 0.0f)
					tFound = true;

				if (minXPlaneIntersection < minimumTValue)
					minimumTValue = minXPlaneIntersection;

				if (minXPlaneIntersection > maximumTValue)
					maximumTValue = minXPlaneIntersection;
			}
		}

		// Intersect ray with the maximum x plane of the aabb.
		plane maxXPlane(vector4(aabbMaxExtents.extractX(), 0.0f, 0.0f, 1.0f), vector4(1.0f, 0.0f, 0.0f, 0.0f));

		float maxXPlaneIntersection;
		if (MathLib::intersectRayWithPlane(intersectionRay, maxXPlane, &maxXPlaneIntersection))
		{
			// Construct the point on the plane. Test if in bounds of the aabb.
			vector4 point;
			vector4_addScaledVector(rayPosition, rayDirection, maxXPlaneIntersection, point);

			if (point.extractY() >= aabbMinExtents.extractY() &&
				point.extractY() <= aabbMaxExtents.extractY() &&
				point.extractZ() >= aabbMinExtents.extractZ() &&
				point.extractZ() <= aabbMaxExtents.extractZ())
			{
				if (maxXPlaneIntersection >= 0.0f)
					tFound = true;

				if (maxXPlaneIntersection < minimumTValue)
					minimumTValue = maxXPlaneIntersection;

				if (maxXPlaneIntersection > maximumTValue)
					maximumTValue = maxXPlaneIntersection;
			}
		}

		// Intersect ray with the minimum y plane of the aabb.
		plane minYPlane(vector4(0.0f, aabbMinExtents.extractY(), 0.0f, 1.0f), vector4(0.0f, -1.0f, 0.0f, 0.0f));

		float minYPlaneIntersection;
		if (MathLib::intersectRayWithPlane(intersectionRay, minYPlane, &minYPlaneIntersection))
		{
			// Construct the point on the plane. Test if in bounds of the aabb.
			vector4 point;
			vector4_addScaledVector(rayPosition, rayDirection, minYPlaneIntersection, point);

			if (point.extractX() >= aabbMinExtents.extractX() &&
				point.extractX() <= aabbMaxExtents.extractX() &&
				point.extractZ() >= aabbMinExtents.extractZ() &&
				point.extractZ() <= aabbMaxExtents.extractZ())
			{
				if (minYPlaneIntersection >= 0.0f)
					tFound = true;

				if (minYPlaneIntersection < minimumTValue)
					minimumTValue = minYPlaneIntersection;

				if (minYPlaneIntersection > maximumTValue)
					maximumTValue = minYPlaneIntersection;
			}
		}

		// Intersect ray with the maximum y plane of the aabb.
		plane maxYPlane(vector4(0.0f, aabbMaxExtents.extractY(), 0.0f, 1.0f), vector4(0.0f, 1.0f, 0.0f, 0.0f));

		float maxYPlaneIntersection;
		if (MathLib::intersectRayWithPlane(intersectionRay, maxYPlane, &maxYPlaneIntersection))
		{
			// Construct the point on the plane. Test if in bounds of the aabb.
			vector4 point;
			vector4_addScaledVector(rayPosition, rayDirection, maxYPlaneIntersection, point);

			if (point.extractX() >= aabbMinExtents.extractX() &&
				point.extractX() <= aabbMaxExtents.extractX() &&
				point.extractZ() >= aabbMinExtents.extractZ() &&
				point.extractZ() <= aabbMaxExtents.extractZ())
			{
				if (maxYPlaneIntersection >= 0.0f)
					tFound = true;

				if (maxYPlaneIntersection < minimumTValue)
					minimumTValue = maxYPlaneIntersection;

				if (maxYPlaneIntersection > maximumTValue)
					maximumTValue = maxYPlaneIntersection;
			}
		}

		// Intersect ray with the minimum z plane of the aabb.
		plane minZPlane(vector4(0.0f, 0.0f, aabbMinExtents.extractZ(), 1.0f), vector4(0.0f, 0.0f, -1.0f, 0.0f));

		float minZPlaneIntersection;
		if (MathLib::intersectRayWithPlane(intersectionRay, minZPlane, &minZPlaneIntersection))
		{
			// Construct the point on the plane. Test if in bounds of the aabb.
			vector4 point;
			vector4_addScaledVector(rayPosition, rayDirection, minZPlaneIntersection, point);

			if (point.extractX() >= aabbMinExtents.extractX() &&
				point.extractX() <= aabbMaxExtents.extractX() &&
				point.extractY() >= aabbMinExtents.extractY() &&
				point.extractY() <= aabbMaxExtents.extractY())
			{
				if (minZPlaneIntersection >= 0.0f)
					tFound = true;

				if (minZPlaneIntersection < minimumTValue)
					minimumTValue = minZPlaneIntersection;

				if (minZPlaneIntersection > maximumTValue)
					maximumTValue = minZPlaneIntersection;
			}
		}

		// Intersect ray with the maximum z plane of the aabb.
		plane maxZPlane(vector4(0.0f, 0.0f, aabbMaxExtents.extractZ(), 1.0f), vector4(0.0f, 0.0f, 1.0f, 0.0f));

		float maxZPlaneIntersection;
		if (MathLib::intersectRayWithPlane(intersectionRay, maxZPlane, &maxZPlaneIntersection))
		{
			// Construct the point on the plane. Test if in bounds of the aabb.
			vector4 point;
			vector4_addScaledVector(rayPosition, rayDirection, maxZPlaneIntersection, point);

			if (point.extractX() >= aabbMinExtents.extractX() &&
				point.extractX() <= aabbMaxExtents.extractX() &&
				point.extractY() >= aabbMinExtents.extractY() &&
				point.extractY() <= aabbMaxExtents.extractY())
			{
				if (maxZPlaneIntersection >= 0.0f)
					tFound = true;

				if (maxZPlaneIntersection < minimumTValue)
					minimumTValue = maxZPlaneIntersection;

				if (maxZPlaneIntersection > maximumTValue)
					maximumTValue = maxZPlaneIntersection;
			}
		}

		if (tFound)
		{
			*tEntry = minimumTValue;
			*tExit = maximumTValue;
		}

		return tFound;
	}
}
