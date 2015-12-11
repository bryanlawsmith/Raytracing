#include "MathLibUtilities.h"
#include "MathLib_MatrixVectorOps.h"
#include "MathLib_QuaternionMatrixOps.h"

namespace MathLib
{

void GenerateTransformMatrix(matrix4x4& transform,
	const vector4& position,
	const vector4& rotation,
	const vector4& scale)
{
    // Generate the translation transform.
    matrix4x4 translationMatrix
    (
        1.0f, 0.0f, 0.0f, position.x,
        0.0f, 1.0f, 0.0f, position.y,
        0.0f, 0.0f, 1.0f, position.z,
        0.0f, 0.0f, 0.0f, 1.0f
    );

    // Generate the rotation transform.
    vector4 xAxis(1.0f, 0.0f, 0.0f, 0.0f);
    vector4 yAxis(0.0f, 1.0f, 0.0f, 0.0f);
    vector4 zAxis(0.0f, 0.0f, 1.0f, 0.0f);

    // Perform rotation around the x axis.
    {
        float angleRads = MATHLIB_DEG_TO_RAD(rotation.x);
        float sinAngle = sinf(angleRads);
        float cosAngle = cosf(angleRads);

        // Caclulate modified y axis.
        vector4 perpendicularContribution;
        vector4_scale(zAxis, sinAngle, perpendicularContribution);
        vector4_scale(yAxis, cosAngle, yAxis);
        vector4_add(yAxis, perpendicularContribution, yAxis);

        // Calculate modified z axis.
        vector4_crossProduct(xAxis, yAxis, zAxis);
    }

    // Perform rotation around the y axis.
    matrix4x4 rotateAroundY;
    rotateAroundY.loadRotationY(rotation.y);
    matrix4x4_vectorMul(rotateAroundY, xAxis, xAxis);
    matrix4x4_vectorMul(rotateAroundY, yAxis, yAxis);
    matrix4x4_vectorMul(rotateAroundY, zAxis, zAxis);

    // Perform rotaiton around the z axis.
    {
        float angleRads = MATHLIB_DEG_TO_RAD(rotation.z);
        float sinAngle = sinf(angleRads);
        float cosAngle = cosf(angleRads);

        // Calculate modified y axis.
        vector4 perpendicularContribution;
        vector4_scale(xAxis, sinAngle, perpendicularContribution);
        vector4_scale(yAxis, cosAngle, yAxis);
        vector4_sub(yAxis, perpendicularContribution, yAxis);

        // Calculate modified x axis.
        vector4_crossProduct(yAxis, zAxis, xAxis);
    }

    matrix4x4 rotationMatrix
    (
        xAxis.x, yAxis.x, zAxis.x, 0.0f,
        xAxis.y, yAxis.y, zAxis.y, 0.0f,
        xAxis.z, yAxis.z, zAxis.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );

    // Generate the scale transform.
    matrix4x4 scaleMatrix
    (
        scale.x, 0.0f, 0.0f, 0.0f,
        0.0f, scale.y, 0.0f, 0.0f,
        0.0f, 0.0f, scale.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );

    // Generate the final transform.
    matrix4x4 scaledRotationMatrix;
    matrix4x4_mul(rotationMatrix, scaleMatrix, scaledRotationMatrix);
    matrix4x4_mul(translationMatrix, scaledRotationMatrix, transform);
}

void GenerateTransformMatrix(matrix4x4& transform,
	const vector4& position,
	const quaternion& orientation,
	const vector4& scale)
{
	// Generation the translation transform.
	matrix4x4 translationTransform
	(
		1.0f, 0.0f, 0.0f, position.x,
		0.0f, 1.0f, 0.0f, position.y,
		0.0f, 0.0f, 1.0f, position.z,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	// Generate the orientation transform.
	matrix4x4 orientationTransform;
	quaternion_toMatrix(orientation, orientationTransform);

	// Gemerate the scale transform.
	matrix4x4 scaleTransform
	(
		scale.x, 0.0f, 0.0f, 0.0f,
		0.0f, scale.y, 0.0f, 0.0f,
		0.0f, 0.0f, scale.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	// Generate the final transform.
	matrix4x4 scaledOrientationTransform;
	matrix4x4_mul(orientationTransform, scaleTransform, scaledOrientationTransform);
	matrix4x4_mul(translationTransform, scaledOrientationTransform, transform);
}

void GenerateInverseTransformMatrix(matrix4x4& transform,
	const vector4& position,
	const quaternion& orientation,
	const vector4& scale)
{
	// Generate the inverse translation transform.
	matrix4x4 invTranslationTransform
	(
		1.0f, 0.0f, 0.0f, -position.x,
		0.0f, 1.0f, 0.0f, -position.y,
		0.0f, 0.0f, 1.0f, -position.z,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	// Generate the inverse orientation transform.
	matrix4x4 invOrientationTransform;
	quaternion invOrientation;
	quaternion_inverse(orientation, invOrientation);
	quaternion_toMatrix(orientation, invOrientationTransform);
	
	// Generate the inverse scale transform.
	matrix4x4 invScaleTransform
	(
		1.0f / scale.x, 0.0f, 0.0f, 0.0f, 
		0.0f, 1.0f / scale.y, 0.0f, 0.0f, 
		0.0f, 0.0f, 1.0f / scale.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	// Generate the final transform.
	matrix4x4 orientationTranslation;
	matrix4x4_mul(invOrientationTransform, invTranslationTransform, orientationTranslation);
	matrix4x4_mul(invScaleTransform, orientationTranslation, transform);
}

void GenerateInverseTransformMatrix(matrix4x4& transform,
	const vector4& position,
	const quaternion& orientation,
	float scale)
{
	GenerateInverseTransformMatrix(transform, position, orientation, 
		MathLib::vector4(scale, scale, scale, 1.0f));
}


void GenerateTransformMatrix(matrix4x4& transform,
    const vector4& position,
    const quaternion& orientation,
    const float scale)
{
    // Generation the translation transform.
    matrix4x4 translationTransform
    (
        1.0f, 0.0f, 0.0f, position.x,
        0.0f, 1.0f, 0.0f, position.y,
        0.0f, 0.0f, 1.0f, position.z,
        0.0f, 0.0f, 0.0f, 1.0f
    );

    // Generate the orientation transform.
    matrix4x4 orientationTransform;
    quaternion_toMatrix(orientation, orientationTransform);

    // Gemerate the scale transform.
    matrix4x4 scaleTransform
    (
        scale, 0.0f, 0.0f, 0.0f,
        0.0f, scale, 0.0f, 0.0f,
        0.0f, 0.0f, scale, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );

    // Generate the final transform.
    matrix4x4 scaledOrientationTransform;
    matrix4x4_mul(orientationTransform, scaleTransform, scaledOrientationTransform);
    matrix4x4_mul(translationTransform, scaledOrientationTransform, transform);
}

}
