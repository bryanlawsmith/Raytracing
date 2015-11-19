#pragma once

#include "GeometryLibCommon.h"
#include <MathLib.h>

using namespace MathLib;

namespace GeometryLib
{
	/// <summary>Determines whether a point is inside an AABB with the given center and half widths.</summary>
	bool PointInsideAABB(const vector4& point, const vector4& center, const vector4& halfWidths);
}