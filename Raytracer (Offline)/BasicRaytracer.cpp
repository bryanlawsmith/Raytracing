#include <cassert>
#include <MathLib.h>
#include "BasicRaytracer.h"
#include "DebugManager.h"
#include "HighPerformanceTimer.h"

using namespace MathLib;
using namespace Core;

namespace Raytracer
{
	BasicRaytracer::BasicRaytracer() : 
		Raytracer()
	{
	}

	BasicRaytracer::BasicRaytracer(FrameBuffer* frameBuffer, Camera* camera, IScene* scene) :
		Raytracer(frameBuffer, camera, scene)
	{
	}

	void BasicRaytracer::Raytrace()
	{
		assert(nullptr != m_Camera);
		assert(nullptr != m_FrameBuffer);

		const float screenWidth = (float)m_FrameBuffer->GetWidth();
		const float screenHeight = (float)m_FrameBuffer->GetHeight();

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

		HighPerformanceTimer timer;
		timer.Start();

		auto& debugManager = Debugging::DebugManager::GetInstance();

		for (int y = 0; y < screenHeight; y++)
		{
			for (int x = 0; x < screenWidth; x++)
			{
				//if (x == (screenWidth * 0.5f) && y == (screenHeight * 0.5f))
				if (x == 472 && y == 457)
					debugManager.SetEnabled(true);

				vector4 screenCoord((float)x, (float)y, 0.0f, 1.0f);
				vector4 cameraSpaceDirection;

				matrix4x4_vectorMul(transform, screenCoord, cameraSpaceDirection);
				vector4_setToVector(cameraSpaceDirection);

				matrix4x4_vectorMul(rotationMatrix, cameraSpaceDirection, cameraSpaceDirection);
				vector4_normalize(cameraSpaceDirection);

				// Transfer position and direction across to ray.
				ray worldSpaceRay;
				worldSpaceRay.setPosition(m_Camera->GetPosition());
				worldSpaceRay.setDirection(cameraSpaceDirection);

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