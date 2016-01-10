#include "Raytracer.h"

namespace Raytracer
{
	Raytracer::Raytracer() : 
		m_FrameBuffer(nullptr), 
		m_Camera(nullptr), 
		m_Scene(nullptr)
	{
	}

	Raytracer::Raytracer(FrameBuffer* frameBuffer,
		Camera* camera,
		IScene* scene) :
		m_FrameBuffer(frameBuffer),
		m_Camera(camera),
		m_Scene(scene)
	{
	}

	void Raytracer::SetFrameBuffer(FrameBuffer* frameBuffer)
	{
		m_FrameBuffer = frameBuffer;
	}

	FrameBuffer* Raytracer::GetFrameBuffer()
	{
		return m_FrameBuffer;
	}

	void Raytracer::SetCamera(Camera* camera)
	{
		m_Camera = camera;
	}

	Camera* Raytracer::GetCamera()
	{
		return m_Camera;
	}

	void Raytracer::SetScene(IScene* scene)
	{
		m_Scene = scene;
	}

	IScene* Raytracer::GetScene()
	{
		return m_Scene;
	}
}