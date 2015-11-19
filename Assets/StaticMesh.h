#pragma once

#include <stdint.h>
#include <memory>

namespace Assets
{
	class StaticMesh
	{
	public:

		StaticMesh(unsigned int numVertices,
				   std::unique_ptr<float[]> vertexArray,
				   std::unique_ptr<float[]> texCoordArray,
				   std::unique_ptr<float[]> normalArray,
				   unsigned int numIndices,
				   std::unique_ptr<uint32_t[]> indicesArray);

		const float* GetVertexArray() const;
		const float* GetTexCoordArray() const;
		const float* GetNormalArray() const;
		const uint32_t* GetIndexArray() const;
		
		unsigned int GetNumVertices() const;
		unsigned int GetNumIndices() const;

	protected:

		StaticMesh(const StaticMesh& rvalue) = delete;
		StaticMesh& operator=(const StaticMesh& rvalue) = delete;

		unsigned int m_NumVertices;
		unsigned int m_NumIndices;

		unsigned int m_VertexArraySizeBytes;
		unsigned int m_TexCoordArraySizeBytes;
		unsigned int m_NormalArraySizeBytes;
		unsigned int m_IndexArraySizeBytes;

		std::unique_ptr<float[]> m_VertexArray;
		std::unique_ptr<float[]> m_NormalArray;
		std::unique_ptr<float[]> m_TexCoordArray;
		std::unique_ptr<uint32_t[]> m_IndexArray;
	};
}