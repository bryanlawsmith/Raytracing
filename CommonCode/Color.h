#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED

#include "Common.h"

namespace CommonCode
{
	struct Color
	{
		float m_R;
		float m_G;
		float m_B;
		float m_A;
	} COMMON_ALIGN(16);
}

#endif COLOR_H_INCLUDED