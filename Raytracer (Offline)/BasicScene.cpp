#include "BasicScene.h"

namespace Raytracer
{
	BasicScene::BasicScene()
	{
	}

	bool BasicScene::Trace(const ray& intersectionRay, float* t, float* results) const
	{
		*t = FLT_MAX;

		bool intersectionFound = false;

		// Naive implementation simply traces through every object in the scene. 
		for (const ITraceable* currentElement : m_Elements)
		{
			float currentT;
			float currentInstanceResults[4];

			if (currentElement->Trace(intersectionRay, &currentT, currentInstanceResults))
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

	void BasicScene::AddTraceable(ITraceable& traceable)
	{
		m_Elements.push_back(&traceable);
	}

	void BasicScene::Clear()
	{
		m_Elements.clear();
	}
}
