#include "IKdTreeTraversal.h"

namespace Raytracer
{
	class KdTreeStackTraversal : public IKdTreeTraversal
	{
	public:

		/// - IKdTreeTraversal Implementation Begin -

		bool Traverse(const KdTreeGeometry& geometry, const ray& intersectionRay, unsigned int* index, float* t, float* u, float* v) override;

		/// - IKdTreeTraversal Implementation End -
	};
}