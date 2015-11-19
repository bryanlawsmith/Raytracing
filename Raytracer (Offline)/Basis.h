#pragma once

#include <MathLib.h>

namespace Raytracer
{
	using namespace MathLib;

	/// <summary>
	/// A basis defines the position, orientation, and scale of an object in 3d space.
	/// </summary>
	struct Basis
	{
		vector4 m_Position;

		quaternion m_Orientation;

		vector4 m_Scale;
	};
}