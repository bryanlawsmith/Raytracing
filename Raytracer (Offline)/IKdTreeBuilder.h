#pragma once

namespace Raytracer
{
	class KdTreeGeometry;

namespace KdTreeConstruction
{
	/// <summary>
	/// Interface defining all of the methods of a Kd tree builder.
	/// </summary>
	class IKdTreeBuilder
	{
	public:
		
		/// <summary>Construct the Kd tree for the provided geometry.</summary>
		virtual void Construct(KdTreeGeometry& geometry) = 0;
	};
}

}