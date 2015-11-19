#include "RaytracerKdTree.h"
#include "FrameBuffer.h"
#include "SceneKdTree.h"
#include "DebugManager.h"
#include <HighPerformanceTimer.h>
#include <cassert>
#include <Camera.h>
#include <MathLib.h>

using namespace MathLib;
using namespace Core;

namespace Raytracer
{
	RaytracerKdTree::RaytracerKdTree()
	{
		m_FrameBuffer = nullptr;
		m_Camera = nullptr;
		m_Scene = new SceneKdTree;
	}

	RaytracerKdTree::~RaytracerKdTree()
	{
		delete m_Scene;
	}

	void RaytracerKdTree::SetFrameBuffer(FrameBuffer* frameBuffer)
	{
		m_FrameBuffer = frameBuffer;
	}

	void RaytracerKdTree::SetCamera(Camera* camera)
	{
		m_Camera = camera;
	}

	IScene& RaytracerKdTree::GetScene()
	{
		return *m_Scene;
	}

	void RaytracerKdTree::Raytrace()
	{
		auto& debugManager = Debugging::DebugManager::GetInstance();

		assert(nullptr != m_Camera);
		assert(nullptr != m_FrameBuffer);

		const float screenWidth = (float)m_FrameBuffer->GetWidth();
		const float screenHeight = (float)m_FrameBuffer->GetHeight();
		const unsigned screenWidthInt = m_FrameBuffer->GetWidth();
		const unsigned screenHeightInt = m_FrameBuffer->GetHeight();

		const float nearClipPlaneDistance = m_Camera->GetNearClipPlaneDistance();
		const float aspectRatio = screenWidth / screenHeight;
		const float halfFov = m_Camera->GetYFov() * 0.5f;

		const float top = nearClipPlaneDistance * tan(MATHLIB_DEG_TO_RAD(halfFov));
		const float right = top * aspectRatio;

		// Now we need to transform the direction into world space.
		vector4& camera_xAxis = m_Camera->GetXAxis();
		vector4& camera_yAxis = m_Camera->GetYAxis();
		vector4& camera_zAxis = m_Camera->GetZAxis();

		matrix4x4 rotationMatrix
		(
			camera_xAxis.extractX(), camera_yAxis.extractX(), camera_zAxis.extractX(), 0.0f,
			camera_xAxis.extractY(), camera_yAxis.extractY(), camera_zAxis.extractY(), 0.0f,
			camera_xAxis.extractZ(), camera_yAxis.extractZ(), camera_zAxis.extractZ(), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);

		// First project the point onto the near clip plane (in camera space of course)
		matrix4x4 screenCoordToNearPlaneB
		(
			right, 0.0f, 0.0f, 0.0f,
			0.0f, top, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);

		matrix4x4 screenCoordToNearPlaneA
		(
			2.0f / (screenWidth - 1.0f), 0.0f, 0.0f, -1.0f,
			0.0f, -2.0f / (screenHeight - 1.0f), 0.0f, +1.0f,
			0.0f, 0.0f, 0.0f, -nearClipPlaneDistance,
			0.0f, 0.0f, 0.0f, 1.0f
		);

		matrix4x4 transform;
		matrix4x4_mul(screenCoordToNearPlaneB, screenCoordToNearPlaneA, transform);

		float* currentFrameBufferPosition = m_FrameBuffer->GetData();

		//getchar();

		HighPerformanceTimer timer;
		timer.Start();

		for (float y = 0.0f; y < screenHeight; y += 1.0f)
		{
			for (float x = 0.0f; x < screenWidth; x += 1.0f)
			{
				if (x == (screenWidth * 0.5f) && y == (screenHeight * 0.5f))
					debugManager.SetEnabled(true);

				vector4 screenCoord(x, y, 0.0f, 1.0f);
				vector4 cameraSpaceDirection;

				matrix4x4_vectorMul(transform, screenCoord, cameraSpaceDirection);
				vector4_setToVector(cameraSpaceDirection);

				matrix4x4_vectorMul(rotationMatrix, cameraSpaceDirection, cameraSpaceDirection);
				vector4_normalize(cameraSpaceDirection);

				// Transfer position and direction across to ray.
				ray worldSpaceRay;
				worldSpaceRay.setPosition(m_Camera->GetPosition());
				worldSpaceRay.setDirection(cameraSpaceDirection);

				currentFrameBufferPosition[0] = 1.0f;
				currentFrameBufferPosition[1] = 1.0f;
				currentFrameBufferPosition[2] = 1.0f;
				currentFrameBufferPosition[3] = 1.0f;

				// Intersect world with ray.
				float t;
				m_Scene->Trace(worldSpaceRay, &t, currentFrameBufferPosition);

				currentFrameBufferPosition += 4;

				//printf("[%i][%i] traced...\n", x, y);
				debugManager.SetEnabled(false);
			}
		}

		timer.Stop();
		printf("Total trace time: %4.2Lf msecs\n", timer.GetTimeMilliseconds());
		printf("Average ray time: %4.5Lf microseconds\n", timer.GetTimeMicroseconds() / (screenWidth * screenHeight));
		getchar();
	}
}