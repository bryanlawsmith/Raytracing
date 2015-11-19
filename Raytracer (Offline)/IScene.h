#pragma once

#include "Basis.h"
#include "ITraceable.h"
#include <StaticMesh.h>
#include <string>

using namespace std;

namespace Raytracer
{
	/// <summary>
	/// Different ray tracer implementations may organize their scenes differently. IScene is the 
	/// common interface to manipulate a scene.
	/// </summary>
	class IScene : public ITraceable
	{
	public:

		virtual void AddStaticMeshInstance(const string& meshId, const Basis& basis) = 0;

		virtual void Reset() = 0;
	};
}