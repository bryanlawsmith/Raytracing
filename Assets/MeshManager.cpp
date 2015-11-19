#include "MeshManager.h"
#include <StoredStaticMesh.h>
#include <cassert>
#include <utility>
#include <stdint.h>

namespace Assets
{
	MeshManager* MeshManager::s_Instance = nullptr;
	MeshManager& MeshManager::GetInstance()
	{
		if (nullptr == MeshManager::s_Instance)
			MeshManager::s_Instance = new MeshManager;

		return *MeshManager::s_Instance;
	}

	MeshManager::MeshManager()
	{
		CreateDefaultMesh();
	}

	MeshManager::~MeshManager()
	{
		if (nullptr != m_MeshDatabase)
			fclose(m_MeshDatabase);
	}

	void MeshManager::SetMeshDatabase(const std::string& fileName)
	{
		fopen_s(&m_MeshDatabase, fileName.c_str(), "rb");
		assert(nullptr != m_MeshDatabase);

		m_MeshDBIndex.Load(m_MeshDatabase);
	}

	const StaticMesh* MeshManager::GetMesh(const std::string& meshName)
	{
		auto meshIterator = m_MeshMap.find(meshName);
		if (m_MeshMap.end() == meshIterator)
		{
			// Attempt to load from the database.
			int offset = m_MeshDBIndex.OffsetOfMesh(meshName);
			if (-1 == offset)
			{
				return m_DefaultMesh.get();
			}
			else
			{
				// We have the offset, we can now load the mesh from the HDD, and initialize its GPU form
				fseek(m_MeshDatabase,
					  static_cast<long>(offset),
					  SEEK_SET);

				StoredStaticMesh storedStaticMesh;
				storedStaticMesh.Load(m_MeshDatabase);

				std::unique_ptr<float[]> vertexArray(storedStaticMesh.GetVertexArray());
				std::unique_ptr<float[]> texCoordArray(storedStaticMesh.GetTexCoordArray());
				std::unique_ptr<float[]> normalArray(storedStaticMesh.GetNormalArray());
				std::unique_ptr<uint32_t[]> indexArray(storedStaticMesh.GetIndexArray());

				auto newMesh = new StaticMesh(storedStaticMesh.GetNumVertices(),
											  std::move(vertexArray),
											  std::move(texCoordArray),
											  std::move(normalArray),
											  storedStaticMesh.GetNumIndices(),
											  std::move(indexArray));

				std::unique_ptr<StaticMesh> staticMesh(newMesh);
				
				m_MeshMap.insert(std::make_pair(meshName, std::move(staticMesh)));

				return newMesh;
			}
		}
		else
		{
			return m_DefaultMesh.get();
		}
	}

	void MeshManager::CreateDefaultMesh()
	{
		unsigned int numVertices = 24;

		std::unique_ptr<float[]> vertexArray(new float[24 * 3]
		{
			// Front face.
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,

			// Right face.
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, 1.0f,

			// Back face.
			1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			// Left face.
			-1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,

			// Top face.
			-1.0f, 1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,

			// Bottom face.
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f
		});

		std::unique_ptr<float[]> texCoordArray(new float[24 * 2]
		{
			// Front face.
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,

			// Right face.
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,

			// Back face.
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,

			// Left face.
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,

			// Top face.
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,

			// Bottom face.
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,
		});

		std::unique_ptr<float[]> normalArray(new float[24 * 3]
		{
			// Front face.
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,

			// Right face.
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,

			// Back face.
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,

			// Left face.
			-1.0f, 0.0f, 0.0f,
			-1.0f, 0.0f, 0.0f,
			-1.0f, 0.0f, 0.0f,
			-1.0f, 0.0f, 0.0f,

			// Top face.
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,

			// Bottom face.
			0.0f, -1.0f, 0.0f,
			0.0f, -1.0f, 0.0f,
			0.0f, -1.0f, 0.0f,
			0.0f, -1.0f, 0.0f
		});

		std::unique_ptr<uint32_t[]> indexArray(new uint32_t[36]
		{
			// Front face.
			0, 1, 3,
			1, 2, 3,

			// Right face.
			4, 5, 7,
			5, 6, 7,

			// Back face.
			8, 9, 11,
			9, 10, 11,

			// Left face.
			12, 13, 15,
			13, 14, 15,

			// Top face.
			16, 17, 19,
			17, 18, 19,

			// Bottom face.
			20, 21, 23,
			21, 22, 23
		});

		m_DefaultMesh = std::unique_ptr<StaticMesh>(new StaticMesh(numVertices, std::move(vertexArray), std::move(texCoordArray), std::move(normalArray), 36, std::move(indexArray)));
	}

	void MeshManager::Reset()
	{
		m_MeshMap.clear();
	}
}