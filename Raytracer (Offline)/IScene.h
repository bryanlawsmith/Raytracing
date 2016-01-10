#pragma once

#include "Basis.h"
#include "ITraceable.h"
#include <StaticMesh.h>
#include <string>

using namespace std;

namespace Raytracer
{
	/// <summary>
	/// Contains a collection of traceable elements. These elements may be 
	/// further organized depending on the type of Scene.
	/// </summary>
	class IScene : public ITraceable
	{
	public:

		virtual bool Trace(const ray& intersectionRay, float* t, float* results) const = 0;

		virtual void AddTraceable(ITraceable& traceable) = 0;

		virtual void Clear() = 0;
	};
}