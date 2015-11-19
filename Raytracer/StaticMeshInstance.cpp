#include "StaticMeshInstance.h"

namespace Raytracer
{
	StaticMeshInstance::StaticMeshInstance(const std::string& meshId,
										   const MathLib::vector4& position,
										   const MathLib::quaternion& orientation,
										   float scale)
	{
		m_MeshId = meshId;
		m_Position = position;
		m_Orientation = orientation;
		m_Scale = scale;
	}

	const std::string& StaticMeshInstance::GetMeshId() const
	{
		return m_MeshId;
	}

	MathLib::vector4& StaticMeshInstance::GetPosition()
	{
		return m_Position;
	}

	const MathLib::vector4& StaticMeshInstance::GetPosition() const
	{
		return m_Position;
	}

	MathLib::quaternion& StaticMeshInstance::GetOrientation()
	{
		return m_Orientation;
	}

	const MathLib::quaternion& StaticMeshInstance::GetOrientation() const
	{
		return m_Orientation;
	}

	float& StaticMeshInstance::GetScale()
	{
		return m_Scale;
	}

	const float& StaticMeshInstance::GetScale() const
	{
		return m_Scale;
	}
}