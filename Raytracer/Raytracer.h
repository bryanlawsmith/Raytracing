#pragma once

#include "Camera.h"
#include "StaticMeshInstance.h"
#include "Triangle.h"
#include <GL/glew.h>
#include <Gl/gl.h>
#include <vector>

namespace Raytracer
{
	class Raytracer
	{
	public:

		Raytracer();
		~Raytracer();

		void Render();

		void ResizeViewport(unsigned int width, unsigned int height);

		void SetCamera(CameraLib::Camera const * camera);

		void ClearSceneData();
		void AppendStaticMeshInstanceToSceneData(const StaticMeshInstance& staticMesh);
		void CreateNewSceneDataSSBO();

	protected:

		/// <summary>
		/// Shader Storage Buffer Object that will contain the scene geometry to be 
		/// used by the raytracer.
		/// </summary>
		GLuint m_SceneGeometrySSBO;

		/// <summary>
		/// Fullscreen quad vertex buffer object.
		/// </summary>
		GLuint m_FullscreenQuadVBO;

		/// <summary>
		/// Fullscreen quad index buffer object.
		/// </summary>
		GLuint m_FullscreenQuadIBO;

		/// <summary>
		/// The texture that the raytracer will write to.
		/// </summary>
		GLuint m_SceneTexture;

		/// <summary>
		/// </summary>
		GLuint m_VertexArrayObject;

		/// <summary>
		/// The main scene camera.
		/// </summary>
		CameraLib::Camera const * m_Camera;

		unsigned int m_ViewportWidth;
		unsigned int m_ViewportHeight;
		
		unsigned int m_NumSceneTriangles;
		unsigned int m_SceneDataMaxTriangles;
		float* m_SceneData;

		void Initialize();

		void RaytraceScene();

		void TransferSceneToFrameBuffer();

		void SubmitTriangleToSceneData(const Triangle& triangle);
	};
}