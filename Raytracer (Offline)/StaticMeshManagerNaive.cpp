#include "StaticMeshManagerNaive.h"
#include "StaticMeshNaive.h"

namespace Raytracer
{
	StaticMeshManagerNaive* StaticMeshManagerNaive::s_Instance = nullptr;
	StaticMeshManagerNaive& StaticMeshManagerNaive::GetInstance()
	{
		if (nullptr == s_Instance)
			s_Instance = new StaticMeshManagerNaive;

		return *s_Instance;
	}

	StaticMeshManagerNaive::StaticMeshManagerNaive()
	{
	}

	const StaticMeshNaive& StaticMeshManagerNaive::GetMesh(const string& id)
	{
		auto item = m_MeshMap.find(id);
		if (item != m_MeshMap.end())
		{
			// Mesh already loaded, return it.
			return *item->second;
		}
		else
		{
			// Load the mesh.
			auto mesh = new StaticMeshNaive(id);
			m_MeshMap.insert(pair<string, StaticMeshNaive*>(id, mesh));

			return *mesh;
		}
	}
}

