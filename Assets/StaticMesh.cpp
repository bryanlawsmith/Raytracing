#include "staticMesh.h"

namespace Assets
{
	StaticMesh::StaticMesh(unsigned int numVertices,
						   std::unique_ptr<float[]> vertexArray,
						   std::unique_ptr<float[]> texCoordArray,
						   std::unique_ptr<float[]> normalArray,
						   unsigned int numIndices,
						   std::unique_ptr<uint32_t[]> indicesArray)
	{
		m_NumVertices = numVertices;
		m_VertexArray = std::move(vertexArray);
		m_TexCoordArray = std::move(texCoordArray);
		m_NormalArray = std::move(normalArray);

		m_NumIndices = numIndices;
		m_IndexArray = std::move(indicesArray);

		m_VertexArraySizeBytes = m_NumVertices * sizeof(float) * 3;
		m_TexCoordArraySizeBytes = m_NumVertices * sizeof(float) * 3;
		m_NormalArraySizeBytes = m_NumVertices * sizeof(float) * 3;
		m_IndexArraySizeBytes = m_NumIndices * sizeof(float) * 3;
	}

	const float* StaticMesh::GetVertexArray() const
	{
		return m_VertexArray.get();
	}

	const float* StaticMesh::GetTexCoordArray() const
	{
		return m_TexCoordArray.get();
	}

	const float* StaticMesh::GetNormalArray() const
	{
		return m_NormalArray.get();
	}

	const uint32_t* StaticMesh::GetIndexArray() const
	{
		return m_IndexArray.get();
	}

	unsigned int StaticMesh::GetNumVertices() const
	{
		return m_NumVertices;
	}

	unsigned int StaticMesh::GetNumIndices() const
	{
		return m_NumIndices;
	}
}