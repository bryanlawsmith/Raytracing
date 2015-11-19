#pragma once

#include <MathLib.h>
#include <string>

namespace Raytracer
{
	class StaticMeshInstance
	{
	public:

		StaticMeshInstance(const std::string& meshId,
						   const MathLib::vector4& position,
						   const MathLib::quaternion& orientation,
						   float scale);

		const std::string& GetMeshId() const;
		
		MathLib::vector4& GetPosition();
		const MathLib::vector4& GetPosition() const;
		
		MathLib::quaternion& GetOrientation();
		const MathLib::quaternion& GetOrientation() const;

		float& GetScale();
		const float& GetScale() const;

	private:

		std::string m_MeshId;

		MathLib::vector4 m_Position;
		MathLib::quaternion m_Orientation;
		float m_Scale;
	};
}