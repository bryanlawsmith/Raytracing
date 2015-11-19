#pragma once

#include "Basis.h"
#include "ITraceable.h"
#include <string>

using namespace std;
using namespace MathLib;

namespace Raytracer
{
	class StaticMeshInstance : public ITraceable
	{
	public:

		StaticMeshInstance(const ITraceable& staticMesh, const Basis& basis);
		StaticMeshInstance(const ITraceable& staticMesh, const vector4& position, const quaternion& orientation, const vector4& scale);

		Basis& GetBasis();
		const Basis& GetBasis() const;

		/// ITraceable implementation begin.

		virtual bool Trace(const ray& intersectionRay, float* t, float* results) const override;

		/// ITraceable implementation end.

	private:

		Basis m_Basis;

		const ITraceable& m_StaticMesh;
	};
}