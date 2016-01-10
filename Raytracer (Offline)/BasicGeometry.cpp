#include "BasicGeometry.h"
#include <MemoryAllocatorAligned.h>
#include <Triangle.h>
#include <Geometry.h>
#include <cassert>
#include <CommonDefines.h>

using namespace Core;

namespace Raytracer
{
	BasicGeometry::BasicGeometry(const StaticMesh& mesh) : 
		m_Triangles(nullptr)
	{
		Initialize(mesh);
	}

	BasicGeometry::~BasicGeometry()
	{
		FreeMemory();
	}

	void BasicGeometry::Initialize(const StaticMesh& mesh)
	{
		FreeMemory();

		auto numVertices = mesh.GetNumVertices();
		auto numIndices = mesh.GetNumIndices();
		assert(0 == numIndices % 3);

		auto vertexArray = mesh.GetVertexArray();
		auto normalArray = mesh.GetNormalArray();
		auto texCoordArray = mesh.GetTexCoordArray();
		auto indexArray = mesh.GetIndexArray();

		// Calculate and allocate required storage space.
		m_NumTriangles = numIndices / 3;
		m_Triangles = reinterpret_cast<Triangle*>(MemoryAllocatorAligned::Allocate((size_t)m_NumTriangles * sizeof(Triangle)));

		for (unsigned i = 0; i < numIndices; i += 3)
		{
			Triangle& currentTriangle = m_Triangles[i / 3];

			for (unsigned v = 0; v < 3; v++)
			{
				auto currentIndex = indexArray[i + v];
				auto currentVertex = vertexArray + (currentIndex * 3);
				auto currentNormal = normalArray + (currentIndex * 3);
				auto currentTexCoord = texCoordArray + (currentIndex * 2);

				currentTriangle.m_Vertices[v].m_Position.setXYZW(currentVertex[0], currentVertex[1], currentVertex[2], 1.0f);
				currentTriangle.m_Vertices[v].m_Normal.setXYZW(currentNormal[0], currentNormal[1], currentNormal[2], 0.0f);
				currentTriangle.m_Vertices[v].m_TexCoord.setXYZW(currentTexCoord[0], currentTexCoord[1], 0.0f, 0.0f);
			}
		}

		GeometryLib::CalculateBoundingVolume(vertexArray, numVertices, m_Bounds[AABB_EXTENTS_MIN], m_Bounds[AABB_EXTENTS_MAX]);
	}

	void BasicGeometry::FreeMemory()
	{
		if (nullptr != m_Triangles)
		{
			MemoryAllocatorAligned::Deallocate(reinterpret_cast<void*>(m_Triangles));
			m_Triangles = nullptr;
		}

		m_NumTriangles = 0;
	}

	bool BasicGeometry::Trace(const ray& intersectionRay, float* t, float* results) const
	{
		bool intersectionFound = false;
		*t = FLT_MAX;

		unsigned int intersectedIndex = 0;
		float intersectedU = 0.0f;
		float intersectedV = 0.0f;

		for (unsigned int i = 0; i < m_NumTriangles; i++)
		{
			float latestT;
			float u;
			float v;

			if (GeometryLib::RayTriangleIntersection(intersectionRay, m_Triangles[i], latestT, u, v))
			{
				intersectionFound = true;
				if (latestT < *t)
				{
					*t = latestT;

					intersectedIndex = i;
					intersectedU = u;
					intersectedV = v;
				}
			}
		}

		if (intersectionFound)
		{
			// Shade result.
			vector4 objectSpaceNormal;
			objectSpaceNormal.setXYZW(0.0f, 0.0f, 0.0f, 0.0f);
			vector4_addScaledVector(objectSpaceNormal, m_Triangles[intersectedIndex].m_Vertices[0].m_Normal, 1.0f - intersectedU - intersectedV, objectSpaceNormal);
			vector4_addScaledVector(objectSpaceNormal, m_Triangles[intersectedIndex].m_Vertices[1].m_Normal, intersectedV, objectSpaceNormal);
			vector4_addScaledVector(objectSpaceNormal, m_Triangles[intersectedIndex].m_Vertices[2].m_Normal, intersectedU, objectSpaceNormal);
			vector4_normalize(objectSpaceNormal);

			float lightFactor = vector4_dotProduct(objectSpaceNormal, vector4(0.0f, 1.0f, 0.0f, 0.0f));
			if (lightFactor > 1.0f) lightFactor = 1.0f;
			if (lightFactor < 0.0f) lightFactor = 0.0f;

			results[0] = lightFactor;
			results[1] = lightFactor;
			results[2] = lightFactor;
			results[3] = 1.0f;

			return true;
		}

		return false;
	}
}