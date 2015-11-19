#pragma once

namespace CameraLib
{
	class Camera;
}

using namespace CameraLib;

namespace Raytracer
{
	class IScene;
	class FrameBuffer;

	/// <summary>
	/// Defines an interface for a raytracer implementation.
	/// </summary>
	class IRaytracer
	{	
		virtual void SetFrameBuffer(FrameBuffer* frameBuffer) = 0;

		virtual void SetCamera(Camera* camera) = 0;

		virtual IScene& GetScene() = 0;

		virtual void Raytrace() = 0;
	};
}