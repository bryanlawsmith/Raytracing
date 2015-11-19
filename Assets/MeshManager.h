#pragma once

#include "StaticMesh.h"
#include <string>
#include <memory>
#include <map>
#include <stdio.h>
#include <StaticMeshDBIndex.h>

namespace Assets
{
	class MeshManager
	{
	public:

		~MeshManager();

		MeshManager& operator=(const MeshManager& rvalue) = delete;
		MeshManager& operator=(MeshManager&& rvalue) = delete;
		
		static MeshManager& GetInstance();

		/// <summary>
		/// Sets the database file associated with this static mesh manager. If a requested mesh instance in not already available, then
		/// this database is searched for the requisite data.
		/// </summary>
		void SetMeshDatabase(const std::string& fileName);

		const StaticMesh* GetMesh(const std::string& meshName);

	private:

		StaticMeshDBIndex m_MeshDBIndex;

		std::unique_ptr<StaticMesh> m_DefaultMesh;

		FILE* m_MeshDatabase;

		std::map<std::string, std::unique_ptr<StaticMesh>> m_MeshMap;

		MeshManager();
		MeshManager(const MeshManager& rvalue) = delete;

		/// <summary>
		/// Frees the memory used by this MeshManager and resets it to a new state.
		/// </summary.
		void Reset();

		/// <summary>
		/// Creates a default mesh that will be used in place when a desired mesh cannot be located.
		/// </summary>
		void CreateDefaultMesh();
		
		static MeshManager* s_Instance;
	};

}