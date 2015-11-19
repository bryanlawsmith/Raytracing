#pragma once

#include <Color.h>

using namespace CommonCode;

namespace Rasterization
{
	struct Line
	{
		int m_X0; 
		int m_Y0;

		int m_X1;
		int m_Y1;

		Color m_Color;
	};
}