#pragma once

#include "GeometryLibCommon.h"
#include <CommonDefines.h>
#include <MathLib.h>

namespace GeometryLib
{
	/// <summary>Calculates the bounding volume of the set of vertices.</summary>
	void CalculateBoundingVolume(const float* vertexArray, unsigned numVertices,
		MathLib::vector4& min, MathLib::vector4& max);

	/// <summary>Transforms a bounding box from object space to world space.</summary>
	void TransformBoundingVolume(const MathLib::vector4& objectSpaceMin,
		const MathLib::vector4& objectSpaceMax, const MathLib::matrix4x4& objectToWorld,
		MathLib::vector4& worldSpaceMin, MathLib::vector4& worldSpaceMax);
}
