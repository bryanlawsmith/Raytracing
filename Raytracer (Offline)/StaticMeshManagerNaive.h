#pragma once

#include <string>
#include <map>

using namespace std;

namespace Raytracer
{
	class StaticMeshNaive;

	class StaticMeshManagerNaive
	{
	public:

		static StaticMeshManagerNaive& GetInstance();

		const StaticMeshNaive& GetMesh(const string& id);

	protected:

		map<string, StaticMeshNaive*> m_MeshMap;
			
		StaticMeshManagerNaive();
		StaticMeshManagerNaive& operator=(const StaticMeshManagerNaive& rvalue) = delete;
		StaticMeshManagerNaive& operator=(StaticMeshManagerNaive&& rvalue) = delete;

		static StaticMeshManagerNaive* s_Instance;
	};
}