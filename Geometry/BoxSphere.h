#pragma once

#include "GeometryLibCommon.h"
#include <MathLib.h>

using namespace MathLib;

namespace GeometryLib
{
	bool SphereInsideBox(const vector4& spherePosition, float sphereRadius, const plane boxPlanes[]);

	bool SphereBoxIntersection(const vector4& spherePosition, float sphereRadius, const plane boxPlanes[]);

	bool BoxInsideSphere(const vector4& spherePosition, float sphereRadius, const vector4 boxVertices[]);
}

