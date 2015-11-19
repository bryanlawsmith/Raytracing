#include "DebugNodeLine.h"
#include "DebugManager.h"
#include "LineDrawing.h"
#include "Rasterization.h"

namespace Debugging
{
	DebugNodeLine::DebugNodeLine(const GeometryLib::Line& line, const Color& color)
	{
		m_Line = line;

		m_Color = color;
	}

	void DebugNodeLine::Process()
	{
		auto& debugManager = DebugManager::GetInstance();
		auto frameBuffer = debugManager.GetFrameBuffer();

		auto& objectToWorldTransform = debugManager.GetObjectToWorldTransform();
		auto& worldViewProjectionTransform = debugManager.GetWorldViewProjectionTransform();
		if (nullptr != frameBuffer)
		{
			matrix4x4 transform;
			MathLib::matrix4x4_mul(worldViewProjectionTransform, objectToWorldTransform, transform);
			Rasterization::RasterizeLine(*frameBuffer, m_Line, transform, m_Color);
		}
	}	
}