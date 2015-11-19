#pragma once

#include "DebugNode.h"
#include <MathLib.h>

using namespace MathLib;

namespace Debugging
{
	/// <summary>
	/// When a transform node is processed, it sets the model->world space transform to be applied to 
	/// all points encountered after that point.
	/// </summary>
	class DebugNodeTransform : public DebugNode
	{
	public:

		DebugNodeTransform(const matrix4x4& transform);

		/// DebugNode implementation begin.

		virtual void Process() override;

		/// DebugNode implementation end.

	protected:
		
		matrix4x4 m_Transform;
	};
}