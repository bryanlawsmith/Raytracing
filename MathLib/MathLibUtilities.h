#ifndef MATHLIBUTILITIES_H_INCLUDED
#define MATHLIBUTILITIES_H_INCLUDED

#include "MathLib_Matrix4x4.h"
#include "MathLib_Quaternion.h"
#include "MathLib_Vector4.h"

namespace MathLib
{
	
/// <summary>
/// Generates the transformation matrix for the given parameters.
/// Rotation elements are euler angles.
/// </summary>
void GenerateTransformMatrix(matrix4x4& transform,
	const vector4& position,
	const vector4& rotation,
	const vector4& scale);

/// <summary>
/// Generates the transformation matrix for the given parameters.
/// </summary>
void GenerateTransformMatrix(matrix4x4& transform,
	const vector4& position,
	const quaternion& orientation,
	const vector4& scale);

/// <summary>
/// Generates the inverse transformation matrix for the given parameters.
/// </summary>
void GenerateInverseTransformMatrix(matrix4x4& transform,
	const vector4& position,
	const quaternion& orientation,
	const vector4& scale);

/// <summary>
/// Generates the inverse transformation matrix for the given parameters.
/// </summary>
void GenerateInverseTransformMatrix(matrix4x4& transform, 
	const vector4& position, 
	const quaternion& orientation, 
	float scale);

/// <summary>
/// Generates the transformation matrix for the given parameters.
/// </summary>
void GenerateTransformMatrix(matrix4x4& transform,
	const vector4& position,
	const quaternion& orientation,
	const float scale);

}

#endif // MATHLIBUTILITIES_H_INCLUDED
