#include "KdTreeGeometry.h"
#include "KdTreeStackTraversal.h"
#include "KdTreeNode.h"
#include "NaiveSpatialMedian.h"
#include "SAH.h"
#include <MemoryAllocatorAligned.h>
#include <Geometry.h>
#include <cassert>

using namespace MathLib;
using namespace Assets;
using namespace Core;

namespace Raytracer
{
	KdTreeGeometry::KdTreeGeometry(const StaticMesh& mesh) :
		m_Triangles(nullptr)
	{
		Initialize(mesh);
	}

	KdTreeGeometry::~KdTreeGeometry()
	{
		FreeMemory();
	}

	void KdTreeGeometry::Initialize(const StaticMesh& mesh)
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

		for (unsigned int i = 0; i < numIndices; i += 3)
		{
			Triangle& currentTriangle = m_Triangles[i / 3];

			for (unsigned int v = 0; v < 3; v++)
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

		// Calculate the bounding volume.
		GeometryLib::CalculateBoundingVolume(vertexArray, numVertices, m_Bounds[AABB_EXTENTS_MIN], m_Bounds[AABB_EXTENTS_MAX]);

		// Construct the kd tree.
		{
			using namespace KdTreeConstruction;
			//KdTreeConstruction::NaiveSpatialMedian kdTreeBuilder;
			KdTreeConstruction::SAH kdTreeBuilder(16, 16);
			kdTreeBuilder.Construct(*this);
		}
	}

	void KdTreeGeometry::FreeMemory()
	{
		if (nullptr != m_Triangles)
		{
			MemoryAllocatorAligned::Deallocate(reinterpret_cast<void*>(m_Triangles));
			m_Triangles = nullptr;
		}

		m_NumTriangles = 0;

		ResetKdTree();
	}

	void KdTreeGeometry::ResetKdTree()
	{
		if (nullptr != m_RootNode)
			delete m_RootNode;

		m_RootNode = nullptr;
	}

	bool KdTreeGeometry::Trace(const ray& intersectionRay, float* t, float* results) const
	{
		IKdTreeTraversal& traversalAlgorithm = KdTreeStackTraversal();

		float u;
		float v;
		unsigned int intersectedTriangleIndex;

		if (traversalAlgorithm.Traverse(*this, intersectionRay, &intersectedTriangleIndex, t, &u, &v))
		{
			// Shade result.
			vector4 objectSpaceNormal;
			objectSpaceNormal.setXYZW(0.0f, 0.0f, 0.0f, 0.0f);
			vector4_addScaledVector(objectSpaceNormal, m_Triangles[intersectedTriangleIndex].m_Vertices[0].m_Normal, 1.0f - u - v, objectSpaceNormal);
			vector4_addScaledVector(objectSpaceNormal, m_Triangles[intersectedTriangleIndex].m_Vertices[1].m_Normal, v, objectSpaceNormal);
			vector4_addScaledVector(objectSpaceNormal, m_Triangles[intersectedTriangleIndex].m_Vertices[2].m_Normal, u, objectSpaceNormal);
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

	Triangle const * KdTreeGeometry::GetTriangles() const
	{
		return m_Triangles;
	}

	unsigned int KdTreeGeometry::GetNumTriangles() const
	{
		return m_NumTriangles;
	}

	KdTreeNode* KdTreeGeometry::GetRootNode() const
	{
		return m_RootNode;
	}
}