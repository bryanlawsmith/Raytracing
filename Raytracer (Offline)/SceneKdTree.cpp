#include "SceneKdTree.h"
#include "StaticMeshManagerKdTree.h"

namespace Raytracer
{
	void SceneKdTree::AddStaticMeshInstance(const string& meshId, const Basis& basis)
	{
		m_StaticMeshInstances.push_back(StaticMeshInstance(static_cast<const ITraceable&>(StaticMeshManagerKdTree::GetInstance().GetMesh(meshId)), basis));
	}

	void SceneKdTree::Reset()
	{
		m_StaticMeshInstances.clear();
	}

	bool SceneKdTree::Trace(const ray& intersectionRay, float* t, float* results) const
	{
		*t = FLT_MAX;

		bool intersectionFound = false;

		// Naive implementation simply traces through every object in the scene. 
		for (const StaticMeshInstance& currentInstance : m_StaticMeshInstances)
		{
			float currentT;
			float currentInstanceResults[4];

			if (currentInstance.Trace(intersectionRay, &currentT, currentInstanceResults))
			{
				if (currentT < *t)
				{
					memcpy(results, currentInstanceResults, sizeof(float) * 4);
					*t = currentT;
				}

				intersectionFound = true;
			}
		}

		return intersectionFound;
	}
}