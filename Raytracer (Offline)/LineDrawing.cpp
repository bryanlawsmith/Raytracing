#include "LineDrawing.h"
#include "FrameBuffer.h"

namespace Rasterization
{
	void DrawLine(FrameBuffer& frameBuffer, const Line& line)
	{
		float* buffer = frameBuffer.GetData();
		unsigned frameBufferWidth = frameBuffer.GetWidth();

		int x = line.m_X0;
		int y = line.m_Y0;

		int dx = line.m_X1 - line.m_X0;
		int dy = line.m_Y1 - line.m_Y0;

		int sx;
		int sy;

		if (dx > 0)
		{
			sx = 1;
		}
		else if (dx < 0)
		{
			sx = -1;
			dx = -dx;
		}
		else
		{
			sx = 0;
		}

		if (dy > 0)
		{
			sy = 1;
		}
		else if (dy < 0)
		{
			sy = -1;
			dy = -dy;
		}
		else
		{
			sy = 0;
		}
		
		int ax = 2 * dx; 
		int ay = 2 * dy;

		if (dy <= dx)
		{
			// Single step in the x-direction.
			for (int decy = ay - dx;; x += sx, decy += ay)
			{
				unsigned bufferOffset = (x + (y * frameBufferWidth)) * 4;
				buffer[bufferOffset + 0] = line.m_Color.m_R;
				buffer[bufferOffset + 1] = line.m_Color.m_G;
				buffer[bufferOffset + 2] = line.m_Color.m_B;
				buffer[bufferOffset + 3] = line.m_Color.m_A;

				// Take step.
				if (x == line.m_X1)
					break;

				if (decy >= 0)
				{
					decy -= ax;
					y += sy;
				}
			}
		}
		else
		{
			// Single step in the y-direction.
			for (int decx = ax - dy;; y += sy, decx += ax)
			{
				unsigned bufferOffset = (x + (y * frameBufferWidth)) * 4;
				buffer[bufferOffset + 0] = line.m_Color.m_R;
				buffer[bufferOffset + 1] = line.m_Color.m_G;
				buffer[bufferOffset + 2] = line.m_Color.m_B;
				buffer[bufferOffset + 3] = line.m_Color.m_A;

				// Take step.
				if (y == line.m_Y1)
					break;

				if (decx >= 0)
				{
					decx -= ay;
					x += sx;
				}
			}
		}
	}
}