#include "DebugNodeTransform.h"
#include "DebugManager.h"

namespace Debugging
{
	DebugNodeTransform::DebugNodeTransform(const matrix4x4& transform)
	{
		MathLib::matrix4x4_copy(m_Transform, transform);
	}

	void DebugNodeTransform::Process()
	{
		auto& debugManager = DebugManager::GetInstance();
		debugManager.SetObjectToWorldTransform(m_Transform);
	}
}