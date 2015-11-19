#include "StaticMeshInstance.h"
#include "DebugManager.h"

using namespace Debugging;

namespace Raytracer
{
	StaticMeshInstance::StaticMeshInstance(const ITraceable& staticMesh, const Basis& basis) : 
		m_StaticMesh(staticMesh)
	{
		m_Basis = basis;
	}

	StaticMeshInstance::StaticMeshInstance(const ITraceable& staticMesh, const vector4& position, const quaternion& orientation, const vector4& scale) : 
		m_StaticMesh(staticMesh)
	{	
		vector4_copy(m_Basis.m_Position, position);
		quaternion_copy(m_Basis.m_Orientation, orientation);
		vector4_copy(m_Basis.m_Scale, scale);
	}

	Basis& StaticMeshInstance::GetBasis()
	{
		return m_Basis;
	}

	const Basis& StaticMeshInstance::GetBasis() const
	{
		return m_Basis;
	}

	bool StaticMeshInstance::Trace(const ray& intersectionRay, float* t, float* results) const
	{
		auto& debugManager = DebugManager::GetInstance();

		// Need to transform ray into object space and translate results back into world space.
		matrix4x4 objectToWorldSpaceTransform;
		MathLib::GenerateTransformMatrix(objectToWorldSpaceTransform, m_Basis.m_Position, m_Basis.m_Orientation, m_Basis.m_Scale);

		matrix4x4 worldToObjectSpaceTransform;
		MathLib::GenerateInverseTransformMatrix(worldToObjectSpaceTransform, m_Basis.m_Position, m_Basis.m_Orientation, m_Basis.m_Scale);

		vector4 rayPosition;
		matrix4x4_vectorMul(worldToObjectSpaceTransform, intersectionRay.getPosition(), rayPosition);

		vector4 rayDirection;
		matrix4x4_vectorMul(worldToObjectSpaceTransform, intersectionRay.getDirection(), rayDirection);
		vector4_normalize(rayDirection);

		ray intersectionRayObjectSpace;
		intersectionRayObjectSpace.setPosition(rayPosition);
		intersectionRayObjectSpace.setDirection(rayDirection);

		if (debugManager.GetEnabled())
			debugManager.AddTransform(objectToWorldSpaceTransform);

		float objectSpaceT;
		float intersectionResults[4];

		bool intersectionFound = m_StaticMesh.Trace(intersectionRayObjectSpace, &objectSpaceT, intersectionResults);
		if (intersectionFound)
		{
			results[0] = intersectionResults[0];
			results[1] = intersectionResults[1];
			results[2] = intersectionResults[2];
			results[3] = intersectionResults[3];

			// Need to calculate intersection point in object space, then transform back into world space to get 
			// world space distance from ray.
			vector4 objectSpaceIntersectionPoint;
			vector4_addScaledVector(rayPosition, rayDirection, objectSpaceT, objectSpaceIntersectionPoint);

			matrix4x4_vectorMul(objectToWorldSpaceTransform, objectSpaceIntersectionPoint, objectSpaceIntersectionPoint);

			*t = vector4_distance(objectSpaceIntersectionPoint, intersectionRay.getPosition());
		}

		return intersectionFound;
	}
}