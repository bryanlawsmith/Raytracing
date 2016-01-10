#include <MathLib.h>

using namespace MathLib;

namespace Raytracer
{
	class KdTreeGeometry;

	/// <summary>
	/// Interface defining the entry point for a Kd tree traversal algorithm.
	/// </summary>
	class IKdTreeTraversal
	{
	public:

		/// <summary>
		/// Traverses the generated kd tree, and determines the intersected triangle index if there is one.
		/// </summary>
		/// <param name="geometry">Geometry containing the kd tree to traverse.</param>
		/// <param name="intersectionRay">The ray to intersect.</param>
		/// <param name="index">If successful, will store the correct triangle's index in the address specified.</param>
		/// <param name="t">If successful, will store the t value of the correct triangle. Used to generate the intersection point.</param>
		/// <param name="u">If successful, will store the barycentric u value of the correct triangle. Used to shade the intersection point.</param>
		/// <param name="v">If successful, will store the barycentric v value of the correct triangle. Used to shade the intersection point.</param>
		/// <return>
		/// True if an intersection was found, false if not.
		/// </return>
		virtual bool Traverse(const KdTreeGeometry& geometry, const ray& intersectionRay, unsigned int* index, float* t, float* u, float* v) = 0;
	};
}