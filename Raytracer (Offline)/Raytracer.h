#pragma once

#include "IScene.h"
#include "FrameBuffer.h"
#include <Camera.h>

using CameraLib::Camera;

namespace Raytracer
{
	class IScene;
	class FrameBuffer;

	/// <summary>
	/// Defines an interface for a raytracer implementation.
	/// The Raytracer class is inherently simple. It is reponsible for raytracing
	/// a scene into a frame buffer, from the perspective of a camera.
	/// </summary>
	class Raytracer
	{	
	public:

		Raytracer();
		Raytracer(FrameBuffer* frameBuffer, Camera* camera, IScene* scene);

		void SetFrameBuffer(FrameBuffer* frameBuffer);
		FrameBuffer* GetFrameBuffer();
		
		void SetCamera(Camera* camera);
		Camera* GetCamera();

		void SetScene(IScene* scene);
		IScene* GetScene();

		virtual void Raytrace() = 0;

	protected:

		FrameBuffer* m_FrameBuffer;
		Camera* m_Camera;
		IScene* m_Scene;
	};
}