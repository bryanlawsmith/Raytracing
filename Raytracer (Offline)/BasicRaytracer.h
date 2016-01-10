#pragma once

#include "Raytracer.h"

namespace Raytracer
{
	/// <summary>
	/// Basic, single-thread implementation of a raytracer.
	/// </summary>
	class BasicRaytracer : public Raytracer
	{
	public:

		BasicRaytracer();
		BasicRaytracer(FrameBuffer* frameBuffer, Camera* camera, IScene* scene);

		void Raytrace() override;
	};
}