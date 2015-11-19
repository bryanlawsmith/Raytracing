#pragma once

#include <string>
#include <map>

using namespace std;

namespace Raytracer
{
	class StaticMeshKdTree;

	class StaticMeshManagerKdTree
	{
	public:

		static StaticMeshManagerKdTree& GetInstance();

		const StaticMeshKdTree& GetMesh(const string& id);

	protected:

		map<string, StaticMeshKdTree*> m_MeshMap;

		StaticMeshManagerKdTree();
		StaticMeshManagerKdTree& operator=(const StaticMeshManagerKdTree& rvalue) = delete;
		StaticMeshManagerKdTree& operator=(StaticMeshManagerKdTree&& rvalue) = delete;

		static StaticMeshManagerKdTree* s_Instance;
	};
}