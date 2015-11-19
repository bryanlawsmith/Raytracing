#pragma once

#include "IRaytracer.h"
#include <map>
#include <string>

using namespace std;

namespace Raytracer
{
	class SceneNaive;
	class StaticMeshNaive;

	class RaytracerNaive : public IRaytracer
	{
	public:

		RaytracerNaive();
		~RaytracerNaive();

		/// IRaytracer implementation begin.

		virtual void SetFrameBuffer(FrameBuffer* frameBuffer) override;

		virtual void SetCamera(Camera* camera) override;

		virtual IScene& GetScene() override;

		virtual void Raytrace() override;

		/// IRaytracer implementation end.

	protected:

		FrameBuffer* m_FrameBuffer;

		Camera* m_Camera;

		SceneNaive* m_Scene;
	};
}