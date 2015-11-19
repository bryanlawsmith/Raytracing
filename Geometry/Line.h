#pragma once

#include "GeometryLibCommon.h"
#include <MathLib.h>

using namespace MathLib;

namespace GeometryLib
{
	struct Line
	{
		vector4 m_P0;
		vector4 m_P1;
	}
	GEOMETRYLIB_ALIGN(16);
}