#pragma once

#include <map>
#include <string>
#include <GenericAlgorithms.h>
#include <MeshManager.h>

using std::string;
using std::map;

namespace Raytracer
{
	/// <summary>
	/// Generic type for storing various Geometry objects.
	/// </summary>
	template <typename GeomType>
	class GeometryCollection
	{
	public:

		GeomType& operator[](const string& id);

	protected:

		map<string, GeomType*> m_Geometry;
	};

	template <typename GeomType>
	GeomType& GeometryCollection<GeomType>::operator[](const string& id)
	{
		using namespace Core;
		using namespace Assets;

		if (!Contains(id, m_Geometry))
		{
			// Load from the mesh database.
			auto& meshManager = MeshManager::GetInstance();
			auto meshData = meshManager.GetMesh(id);

			auto newElement = new GeomType(*meshData);
			m_Geometry[id] = newElement;

			delete meshData;
		}

		return *m_Geometry[id];
	}
}