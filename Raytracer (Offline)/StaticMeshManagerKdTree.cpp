#include "StaticMeshManagerKdTree.h"
#include "StaticMeshKdTree.h"

namespace Raytracer
{
	StaticMeshManagerKdTree* StaticMeshManagerKdTree::s_Instance = nullptr;
	StaticMeshManagerKdTree& StaticMeshManagerKdTree::GetInstance()
	{
		if (nullptr == s_Instance)
			s_Instance = new StaticMeshManagerKdTree;

		return *s_Instance;
	}

	StaticMeshManagerKdTree::StaticMeshManagerKdTree()
	{

	}

	const StaticMeshKdTree& StaticMeshManagerKdTree::GetMesh(const string& id)
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
			auto mesh = new StaticMeshKdTree(id);
			m_MeshMap.insert(pair<string, StaticMeshKdTree*>(id, mesh));

			return *mesh;
		}
	}
}