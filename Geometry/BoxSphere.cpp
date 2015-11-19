#include "BoxSphere.h"

namespace GeometryLib
{
	bool SphereInsideBox(const vector4& spherePosition, float sphereRadius, const plane boxPlanes[])
	{
		for (unsigned int i = 0; i < 6; i++)
			if (!MathLib::sphereInsidePlane(spherePosition, sphereRadius, boxPlanes[i]))
				return false;

		return true;
	}

	bool SphereBoxIntersection(const vector4& spherePosition, float sphereRadius, const plane boxPlanes[])
	{
		bool inLeft = !sphereOutsidePlane(spherePosition, sphereRadius, boxPlanes[BOX_FACES_LEFT]);
		bool inRight = !sphereOutsidePlane(spherePosition, sphereRadius, boxPlanes[BOX_FACES_RIGHT]);
		bool inTop = !sphereOutsidePlane(spherePosition, sphereRadius, boxPlanes[BOX_FACES_TOP]);
		bool inBottom = !sphereOutsidePlane(spherePosition, sphereRadius, boxPlanes[BOX_FACES_BOTTOM]);
		bool inFront = !sphereOutsidePlane(spherePosition, sphereRadius, boxPlanes[BOX_FACES_FRONT]);
		bool inBack = !sphereOutsidePlane(spherePosition, sphereRadius, boxPlanes[BOX_FACES_BACK]);

		if (sphereIntersectsPlane(spherePosition, sphereRadius, boxPlanes[BOX_FACES_TOP]) && inLeft && inRight && inFront && inBack)
			return true;

		if (sphereIntersectsPlane(spherePosition, sphereRadius, boxPlanes[BOX_FACES_BOTTOM]) &&	inLeft && inRight && inFront && inBack)
			return true;

		if (sphereIntersectsPlane(spherePosition, sphereRadius, boxPlanes[BOX_FACES_LEFT]) && inTop && inBottom && inFront && inBack)
			return true;

		if (sphereIntersectsPlane(spherePosition, sphereRadius, boxPlanes[BOX_FACES_RIGHT]) && inTop && inBottom && inFront && inBack)
			return true;

		if (sphereIntersectsPlane(spherePosition, sphereRadius, boxPlanes[BOX_FACES_FRONT]) && inTop && inBottom && inLeft && inRight)
			return true;

		if (sphereIntersectsPlane(spherePosition, sphereRadius, boxPlanes[BOX_FACES_BACK]) && inTop && inBottom && inLeft && inRight)
			return true;

		return false;
	}

	bool SphereOutsideBox(const vector4& spherePosition, float sphereRadius, const plane boxPlanes[])
	{
		return !(SphereInsideBox(spherePosition, sphereRadius, boxPlanes) || SphereBoxIntersection(spherePosition, sphereRadius, boxPlanes));
	}

	bool BoxInsideSphere(const vector4& spherePosition, float sphereRadius, const vector4 boxVertices[])
	{
		for (unsigned int i = 0; i < 8; i++)
			if (vector4_distance(spherePosition, boxVertices[i]) > sphereRadius)
				return false;

		return true;
	}
}

