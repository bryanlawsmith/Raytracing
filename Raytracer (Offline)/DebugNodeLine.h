#pragma once

#include "DebugNode.h"
#include <Color.h>
#include <Line.h>

using namespace CommonCode;

namespace Debugging
{
	class DebugNodeLine : public DebugNode
	{
	public:

		DebugNodeLine(const GeometryLib::Line& line, const Color& color);

	protected:

		GeometryLib::Line m_Line;

		Color m_Color;

		/// DebugNode implementation begin.

		virtual void Process() override;

		/// DebugNode implementation end.
	};
}