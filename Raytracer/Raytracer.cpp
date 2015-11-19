#include "Raytracer.h"
#include "ShaderManager.h"
#include <Meshmanager.h>
#include <MathLib.h>
#include <MathUtil.h>

namespace Raytracer
{
	Raytracer::Raytracer()
	{
		Initialize();
	}

	Raytracer::~Raytracer()
	{
		delete [] m_SceneData;
	}

	void Raytracer::ResizeViewport(unsigned int width, unsigned int height)
	{
		glViewport(0, 0, width, height);

		// Adjust camera viewport options.
		m_ViewportWidth = width;
		m_ViewportHeight = height;
	}

	void Raytracer::SetCamera(CameraLib::Camera const * camera)
	{
		m_Camera = camera;
	}

	void Raytracer::Render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RaytraceScene();

		TransferSceneToFrameBuffer();
	}

	void Raytracer::Initialize()
	{
		// Set fields.
		m_Camera = nullptr;

		// Initialize the shaders.
		auto& shaderManager = ShaderManager::GetInstance();

		glDisable(GL_CULL_FACE);

		glClearDepth(1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		// Create the scene information.
		{
			m_SceneDataMaxTriangles = 20000;
			m_SceneData = new float[m_SceneDataMaxTriangles * sizeof(Triangle) / sizeof(float)];
			m_NumSceneTriangles = 0;
		}

		// Create the full screen quad buffers.
		{
			GLfloat vertexData[] =
			{
				// Vertex data.
				-1.0f, 1.0f,
				1.0f, 1.0f,
				1.0f, -1.0f,
				-1.0f, -1.0f,

				// Texture data.
				0.0f, 1.0f,
				1.0f, 1.0f,
				1.0f, 0.0f,
				0.0f, 0.0f
			};

			GLushort indexData[] =
			{
				0, 1, 2,
				0, 2, 3
			};

			glGenVertexArrays(1, &m_VertexArrayObject);
			glBindVertexArray(m_VertexArrayObject);

			glGenBuffers(1, &m_FullscreenQuadVBO);
			glBindBuffer(GL_ARRAY_BUFFER, m_FullscreenQuadVBO);
			glBufferData(GL_ARRAY_BUFFER,
						 sizeof(vertexData),
						 vertexData,
						 GL_STATIC_DRAW);

			glGenBuffers(1, &m_FullscreenQuadIBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_FullscreenQuadIBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER,
						 sizeof(indexData),
						 indexData,
						 GL_STATIC_DRAW);

			// Bind vertex attribute locations.
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0,
								  2,
								  GL_FLOAT,
								  GL_FALSE,
								  0,
								  0);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1,
								  2,
								  GL_FLOAT,
								  GL_FALSE,
								  0,
								  reinterpret_cast<void*>(sizeof(GLfloat)* 8));

			glBindVertexArray(0);
		}

		// Create the scene texture.
		{
			glGenTextures(1, &m_SceneTexture);
			glBindTexture(GL_TEXTURE_2D, m_SceneTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTexImage2D(GL_TEXTURE_2D,
						 0,
						 GL_RGBA32F,
						 1024,
						 600,
						 0,
						 GL_RGBA,
						 GL_FLOAT,
						 nullptr);

			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	void Raytracer::RaytraceScene()
	{
		auto& shaderManager = ShaderManager::GetInstance();
		auto shader = shaderManager.GetShader(ShaderManager::SHADER_CORE_TRACE);
		shader->Use();

		glEnable(GL_TEXTURE_2D);
		glBindImageTexture(0, m_SceneTexture, 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);

		// Ray generation matrix.
		{
			float aspectRatio = (float)m_ViewportWidth / (float)m_ViewportHeight;

			float halfYFov = m_Camera->GetYFov() * 0.5f;

			float nearPlaneDistance = m_Camera->GetNearClipPlaneDistance();
			float top = nearPlaneDistance * tan(MATHLIB_DEG_TO_RAD(halfYFov));
			float right = top * aspectRatio;

			MathLib::matrix4x4 screenCoordToNearPlaneScale
			(
				right, 0.0f, 0.0f, 0.0f,
				0.0f, top, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);

			MathLib::matrix4x4 screenCoordToNearPlaneNormalize
			(
				2.0f / (m_ViewportWidth - 1.0f), 0.0f, 0.0f, -1.0f,
				0.0f, -2.0f / (m_ViewportHeight - 1.0f), 0.0f, +1.0f,
				0.0f, 0.0f, 0.0f, -nearPlaneDistance, 
				0.0f, 0.0f, 0.0f, 1.0f
			);

			MathLib::matrix4x4 screenCoordTransform;
			MathLib::matrix4x4_mul(screenCoordToNearPlaneScale, screenCoordToNearPlaneNormalize, screenCoordTransform);
			
			// Now we need to transform the direction into world space.
			auto& cameraXAxis = m_Camera->GetXAxis();
			auto& cameraYAxis = m_Camera->GetYAxis();
			auto& cameraZAxis = m_Camera->GetZAxis();

			MathLib::matrix4x4 rotationTransform
			(
				cameraXAxis.extractX(), cameraYAxis.extractX(), cameraZAxis.extractX(), 0.0f,
				cameraXAxis.extractY(), cameraYAxis.extractY(), cameraZAxis.extractY(), 0.0f,
				cameraXAxis.extractZ(), cameraYAxis.extractZ(), cameraZAxis.extractZ(), 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);

			GLfloat matrixArray[16];
			screenCoordTransform.setOpenGLMatrix(matrixArray);
			shader->SetUniformMatrix4fv("u_ScreenCoordTransform", matrixArray);

			rotationTransform.setOpenGLMatrix(matrixArray);
			shader->SetUniformMatrix4fv("u_RotationTransform", matrixArray);

			auto& cameraPosition = m_Camera->GetPosition();

			shader->SetUniform4f("u_CameraPosition",
								 cameraPosition.extractX(),
								 cameraPosition.extractY(),
								 cameraPosition.extractZ(), 
								 1.0f);
		}

		// Set up scene triangle buffer.
		{
			shader->SetUniform1i("u_NumSceneTriangles", (GLint)m_NumSceneTriangles);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_SceneGeometrySSBO);
		}

		glDispatchCompute(m_ViewportWidth / 16, m_ViewportHeight / 16, 1);
		glMemoryBarrier(GL_ALL_BARRIER_BITS);

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
		glBindImageTexture(0, 0, 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);
	}

	void Raytracer::TransferSceneToFrameBuffer()
	{
		auto& shaderManager = ShaderManager::GetInstance();
		auto shader = shaderManager.GetShader(ShaderManager::SHADER_SIMPLE_TEXTURE);
		shader->Use();

		glBindVertexArray(m_VertexArrayObject);

		// Set transform.
		{
			MathLib::matrix4x4 matrix;
			matrix.loadIdentity();

			GLfloat matrixArray[16];
			matrix.setOpenGLMatrix(matrixArray);
			shader->SetUniformMatrix4fv("u_Transform", matrixArray);
		}

		shader->SetUniform1i("u_Texture", 0);

		auto shaderProgramHandle = shader->GetProgramHandle();

		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_SceneTexture);

		glDrawElements(GL_TRIANGLES,
					   6, 
					   GL_UNSIGNED_SHORT, 
					   0);

		glDisable(GL_TEXTURE_2D);
	}

	void Raytracer::AppendStaticMeshInstanceToSceneData(const StaticMeshInstance& staticMesh)
	{
		using namespace Assets;

		auto& meshManager = MeshManager::GetInstance();

		auto meshData = meshManager.GetMesh(staticMesh.GetMeshId());
		auto numVertices = meshData->GetNumVertices();
		auto numIndices = meshData->GetNumIndices();

		auto vertexArray = meshData->GetVertexArray();
		auto texCoordArray = meshData->GetTexCoordArray();
		auto normalArray = meshData->GetNormalArray();
		auto indexArray = meshData->GetIndexArray();

		MathLib::matrix4x4 transform;
		MathLib::GenerateTransformMatrix(transform,
										 staticMesh.GetPosition(),
										 staticMesh.GetOrientation(),
										 staticMesh.GetScale());

		for (unsigned int i = 0; i < numIndices; i += 3)
		{
			Triangle triangle;
			for (unsigned int v = 0; v < 3; v++)
			{
				// We need to transform all of the vertices and normals into their world space representation before 
				// sending them to the raytracer. 
				auto offset0 = (indexArray[i + v]) * 3;
				auto offset1 = offset0 + 1;
				auto offset2 = offset1 + 1;

				MathLib::vector4 vertex(vertexArray[offset0], vertexArray[offset1], vertexArray[offset2], 1.0f);
				MathLib::matrix4x4_vectorMul(transform, vertex, vertex);

				triangle.m_Vertices[v].m_Position[0] = vertex.extractX();
				triangle.m_Vertices[v].m_Position[1] = vertex.extractY();
				triangle.m_Vertices[v].m_Position[2] = vertex.extractZ();

				MathLib::vector4 normal(normalArray[offset0], normalArray[offset1], normalArray[offset2], 0.0f);
				MathLib::matrix4x4_vectorMul(transform, normal, normal);
				MathLib::vector4_normalize(normal);

				triangle.m_Vertices[v].m_Normal[0] = normal.extractX();
				triangle.m_Vertices[v].m_Normal[1] = normal.extractY();
				triangle.m_Vertices[v].m_Normal[2] = normal.extractZ();
			}
			SubmitTriangleToSceneData(triangle);
		}
	}

	void Raytracer::SubmitTriangleToSceneData(const Triangle& triangle)
	{
		// Get the current offset into the buffer.
		auto startOffset = m_NumSceneTriangles * sizeof(Triangle);
		auto offset = startOffset / sizeof(float);
		
		for (unsigned int currentVertex = 0; currentVertex < 3; currentVertex++)
		{
			m_SceneData[offset++] = triangle.m_Vertices[currentVertex].m_Position[0];
			m_SceneData[offset++] = triangle.m_Vertices[currentVertex].m_Position[1];
			m_SceneData[offset++] = triangle.m_Vertices[currentVertex].m_Position[2];
			m_SceneData[offset++] = triangle.m_Vertices[currentVertex].m_Position[3];
		
			m_SceneData[offset++] = triangle.m_Vertices[currentVertex].m_Normal[0];
			m_SceneData[offset++] = triangle.m_Vertices[currentVertex].m_Normal[1];
			m_SceneData[offset++] = triangle.m_Vertices[currentVertex].m_Normal[2];
			m_SceneData[offset++] = triangle.m_Vertices[currentVertex].m_Normal[3];
		}

		m_NumSceneTriangles++;
	}

	void Raytracer::ClearSceneData()
	{
		m_NumSceneTriangles = 0;
	}

	void Raytracer::CreateNewSceneDataSSBO()
	{
		glDeleteBuffers(1, &m_SceneGeometrySSBO);

		glGenBuffers(1, &m_SceneGeometrySSBO);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SceneGeometrySSBO);
		glBufferData(GL_SHADER_STORAGE_BUFFER, m_SceneDataMaxTriangles * sizeof(Triangle), m_SceneData, GL_STATIC_DRAW);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
}