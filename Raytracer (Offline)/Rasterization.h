#pragma once

#include "FrameBuffer.h"
#include <MathLib.h>
#include <Geometry.h>
#include <Color.h>

using namespace CommonCode;

namespace Rasterization
{
	/// <summary>
	/// Scales the provided coordinates from NDC space to window space.
	/// </summary>
	void ScaleToWindowCoordinates(float& xNDC, float& yNDC, float viewportWidth, float viewportHeight);

	/// <summary>
	/// Clamps the provided point to the window space region.
	/// </summary>
	void ClampToWindowCoordinates(int& x, int& y, unsigned viewportWidth, unsigned viewportHeight);

	/// <summary>
	/// Rasterizes a line to the specified framebuffer.
	/// </summary>
	void RasterizeLine(Raytracer::FrameBuffer& frameBuffer, const GeometryLib::Line& line, const MathLib::matrix4x4& transform, const Color& color);
}