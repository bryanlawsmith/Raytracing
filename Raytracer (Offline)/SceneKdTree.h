#pragma once 

#include "IScene.h"
#include "StaticMeshKdTree.h"
#include "StaticMeshInstance.h"
#include <vector>
#include <map>
#include <string>

using namespace std;

namespace Raytracer
{
	class SceneKdTree : public IScene
	{
	public:

		/// IScene implementation start.

		virtual void AddStaticMeshInstance(const string& meshId, const Basis& basis) override;

		virtual void Reset() override;

		/// IScene implementation end.

		/// ITraceable implementation start.

		virtual bool Trace(const ray& intersectionRay, float* t, float* results) const override;

		/// ITraceable implementation end.

	protected:

		vector<StaticMeshInstance> m_StaticMeshInstances;
	};
}