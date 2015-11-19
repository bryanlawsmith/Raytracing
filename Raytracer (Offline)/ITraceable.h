#pragma once

#include <MathLib.h>

using namespace MathLib;

namespace Raytracer
{
	class ITraceable
	{
	public:

		virtual bool Trace(const ray& intersectionRay, float* t, float* results) const = 0;
	};
}