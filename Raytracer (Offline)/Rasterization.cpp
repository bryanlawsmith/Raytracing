#include "Rasterization.h"
#include "Line.h"
#include "LineDrawing.h"

namespace Rasterization
{
	void ScaleToWindowCoordinates(float& xNDC, float& yNDC, float viewportWidth, float viewportHeight)
	{
		xNDC = ((xNDC + 1.0f) * 0.5f) * viewportWidth;
		yNDC = (1.0f - ((yNDC + 1.0f) * 0.5f)) * viewportHeight;
	}

	void ClampToWindowCoordinates(int& x, int& y, unsigned viewportWidth, unsigned viewportHeight)
	{
		int maxX = (int)viewportWidth - 1;
		int maxY = (int)viewportHeight - 1;
		
		if (x < 0) x = 0;
		else if (x > maxX) x = maxX;

		if (y < 0) y = 0;
		else if (y > maxY) y = maxY;
	}

	void RasterizeLine(Raytracer::FrameBuffer& frameBuffer, const GeometryLib::Line& line, const MathLib::matrix4x4& transform, const Color& color)
	{
		const unsigned frameBufferWidth = frameBuffer.GetWidth();
		const unsigned frameBufferHeight = frameBuffer.GetHeight();
		const float frameBufferWidthf = (float)frameBufferWidth;
		const float frameBufferHeightf = (float)frameBufferHeight;

		// Transform the line from object space into clip space.
		vector4 p0Transformed;
		vector4 p1Transformed;

		MathLib::matrix4x4_vectorMul(transform, line.m_P0, p0Transformed);
		MathLib::matrix4x4_vectorMul(transform, line.m_P1, p1Transformed);

		// Project points into NDC space.
		float p0ZRecip = 1.0f / p0Transformed.extractW();
		float p1ZRecip = 1.0f / p1Transformed.extractW();

		MathLib::vector4_scale(p0Transformed, p0ZRecip, p0Transformed);
		MathLib::vector4_scale(p1Transformed, p1ZRecip, p1Transformed);

		// Scale from NDC space to window coordinates.
		float x0 = p0Transformed.extractX();
		float y0 = p0Transformed.extractY();
		Rasterization::ScaleToWindowCoordinates(x0, y0, frameBufferWidthf, frameBufferHeightf);

		float x1 = p1Transformed.extractX();
		float y1 = p1Transformed.extractY();
		Rasterization::ScaleToWindowCoordinates(x1, y1, frameBufferWidthf, frameBufferHeightf);

		Rasterization::Line windowLine;
		windowLine.m_X0 = (int)x0;
		windowLine.m_Y0 = (int)y0;

		windowLine.m_X1 = (int)x1;
		windowLine.m_Y1 = (int)y1;

		windowLine.m_Color = color;

		Rasterization::ClampToWindowCoordinates(windowLine.m_X0, windowLine.m_Y0, frameBufferWidth, frameBufferHeight);
		Rasterization::ClampToWindowCoordinates(windowLine.m_X1, windowLine.m_Y1, frameBufferWidth, frameBufferHeight);

		Rasterization::DrawLine(frameBuffer, windowLine);
	}
}