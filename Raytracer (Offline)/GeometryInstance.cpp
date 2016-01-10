#include <Geometry.h>
#include "GeometryInstance.h"
#include "DebugManager.h"

using namespace Debugging;
using namespace MathLib;

namespace Raytracer
{
	GeometryInstance::GeometryInstance(const BoundedTraceable& geometry, const Basis& basis) :
		m_Geometry(geometry)
	{
		m_Basis = basis;

		Update();
	}

	GeometryInstance::GeometryInstance(const BoundedTraceable& geometry, const vector4& position,
		const quaternion& orientation, const vector4& scale) :
		m_Geometry(geometry)
	{	
		vector4_copy(m_Basis.m_Position, position);
		quaternion_copy(m_Basis.m_Orientation, orientation);
		vector4_copy(m_Basis.m_Scale, scale);

		Update();
	}

	Basis& GeometryInstance::GetBasis()
	{
		return m_Basis;
	}

	const Basis& GeometryInstance::GetBasis() const
	{
		return m_Basis;
	}

	void GeometryInstance::SetBasis(const Basis& basis)
	{
		m_Basis = basis;
		Update();
	}

	bool GeometryInstance::Trace(const ray& intersectionRay, float* t, float* results) const
	{
		auto& debugManager = DebugManager::GetInstance();

		vector4 rayPosition;
		matrix4x4_vectorMul(m_WorldToObject, intersectionRay.getPosition(), rayPosition);

		vector4 rayDirection;
		matrix4x4_vectorMul(m_WorldToObject, intersectionRay.getDirection(), rayDirection);
		vector4_normalize(rayDirection);

		ray intersectionRayObjectSpace;
		intersectionRayObjectSpace.setPosition(rayPosition);
		intersectionRayObjectSpace.setDirection(rayDirection);

		if (debugManager.GetEnabled())
			debugManager.AddTransform(m_ObjectToWorld);

		float objectSpaceT;
		float intersectionResults[4];

		bool intersectionFound = m_Geometry.Trace(intersectionRayObjectSpace, &objectSpaceT,
			intersectionResults);
		if (intersectionFound)
		{
			results[0] = intersectionResults[0];
			results[1] = intersectionResults[1];
			results[2] = intersectionResults[2];
			results[3] = intersectionResults[3];

			// Need to calculate intersection point in object space, then transform back into
			// world space to get world space distance from ray.
			vector4 objectSpaceIntersectionPoint;
			vector4_addScaledVector(rayPosition, rayDirection, objectSpaceT, 
				objectSpaceIntersectionPoint);

			matrix4x4_vectorMul(m_ObjectToWorld, objectSpaceIntersectionPoint, 
				objectSpaceIntersectionPoint);

			*t = vector4_distance(objectSpaceIntersectionPoint, intersectionRay.getPosition());
		}

		return intersectionFound;
	}

	void GeometryInstance::Update()
	{
		// Recalculate matrices.
		MathLib::GenerateTransformMatrix(m_ObjectToWorld, m_Basis.m_Position, m_Basis.m_Orientation,
			m_Basis.m_Scale);
		MathLib::GenerateInverseTransformMatrix(m_WorldToObject, m_Basis.m_Position, m_Basis.m_Orientation,
			m_Basis.m_Scale);

		// Recalculate the world-space AABB.
		auto objectSpaceBounds = m_Geometry.GetBounds();
		GeometryLib::TransformBoundingVolume(objectSpaceBounds[AABB_EXTENTS_MIN],
			objectSpaceBounds[AABB_EXTENTS_MAX], m_ObjectToWorld, m_Bounds[AABB_EXTENTS_MIN],
			m_Bounds[AABB_EXTENTS_MAX]);
	}
}