#pragma once 

#include "Line.h"

namespace Raytracer
{
	class FrameBuffer;
}

using namespace Raytracer;

namespace Rasterization
{
	void DrawLine(FrameBuffer& frameBuffer, const Line& line);
}