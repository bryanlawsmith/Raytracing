#include "BoundingVolumeCalculation.h"

namespace GeometryLib
{
	void CalculateBoundingVolume(const float* vertexArray, unsigned numVertices,
		MathLib::vector4& min, MathLib::vector4& max)
	{
		min.setXYZW(FLT_MAX, FLT_MAX, FLT_MAX, 1.0f);
		max.setXYZW(-FLT_MAX, -FLT_MAX, -FLT_MAX, 1.0f);

		for (unsigned i = 0; i < numVertices; i++)
		{
			auto currentVertex = vertexArray + (i * 3);
			if (currentVertex[0] < min.x) min.x = currentVertex[0];
			if (currentVertex[1] < min.y) min.y = currentVertex[1];
			if (currentVertex[2] < min.z) min.z = currentVertex[2];

			if (currentVertex[0] > max.x) max.x = currentVertex[0];
			if (currentVertex[1] > max.y) max.y = currentVertex[1];
			if (currentVertex[2] > max.z) max.z = currentVertex[2];
		}
	}

	void TransformBoundingVolume(const MathLib::vector4& objectSpaceMin,
		const MathLib::vector4& objectSpaceMax, const MathLib::matrix4x4& objectToWorld,
		MathLib::vector4& worldSpaceMin, MathLib::vector4& worldSpaceMax)
	{
		objectToWorld.getPositionComponent(worldSpaceMin);
		objectToWorld.getPositionComponent(worldSpaceMax);

		// Calculate min/max x.
		{
			if (objectToWorld._00 > 0.0f)
			{
				worldSpaceMin.x += objectToWorld._00 * objectSpaceMin.x;
				worldSpaceMax.x += objectToWorld._00 * objectSpaceMax.x;
			}
			else
			{
				worldSpaceMin.x += objectToWorld._00 * objectSpaceMax.x;
				worldSpaceMax.x += objectToWorld._00 * objectSpaceMin.x;
			}

			if (objectToWorld._01 > 0.0f)
			{
				worldSpaceMin.x += objectToWorld._01 * objectSpaceMin.y;
				worldSpaceMax.x += objectToWorld._01 * objectSpaceMax.y;
			}
			else
			{
				worldSpaceMin.x += objectToWorld._01 * objectSpaceMax.y;
				worldSpaceMax.x += objectToWorld._01 * objectSpaceMin.y;
			}

			if (objectToWorld._02 > 0.0f)
			{
				worldSpaceMin.x += objectToWorld._02 * objectSpaceMin.z;
				worldSpaceMax.x += objectToWorld._02 * objectSpaceMax.z;
			}
			else
			{
				worldSpaceMin.x += objectToWorld._02 * objectSpaceMax.z;
				worldSpaceMax.x += objectToWorld._02 * objectSpaceMin.z;
			}
		}

		// Calculate min/max y.
		{
			if (objectToWorld._10 > 0.0f)
			{
				worldSpaceMin.y += objectToWorld._10 * objectSpaceMin.x;
				worldSpaceMax.y += objectToWorld._10 * objectSpaceMax.x;
			}
			else
			{
				worldSpaceMin.y += objectToWorld._10 * objectSpaceMax.x;
				worldSpaceMax.y += objectToWorld._10 * objectSpaceMin.x;
			}

			if (objectToWorld._11 > 0.0f)
			{
				worldSpaceMin.y += objectToWorld._11 * objectSpaceMin.y;
				worldSpaceMax.y += objectToWorld._11 * objectSpaceMax.y;
			}
			else
			{
				worldSpaceMin.y += objectToWorld._11 * objectSpaceMax.y;
				worldSpaceMax.y += objectToWorld._11 * objectSpaceMin.y;
			}

			if (objectToWorld._12 > 0.0f)
			{
				worldSpaceMin.y += objectToWorld._12 * objectSpaceMin.z;
				worldSpaceMax.y += objectToWorld._12 * objectSpaceMax.z;
			}
			else
			{
				worldSpaceMin.y += objectToWorld._12 * objectSpaceMax.z;
				worldSpaceMax.y += objectToWorld._12 * objectSpaceMin.z;
			}
		}

		// Calculate min/max z.
		{
			if (objectToWorld._20 > 0.0f)
			{
				worldSpaceMin.z += objectToWorld._20 * objectSpaceMin.x;
				worldSpaceMax.z += objectToWorld._20 * objectSpaceMax.x;
			}
			else
			{
				worldSpaceMin.z += objectToWorld._20 * objectSpaceMax.x;
				worldSpaceMax.z += objectToWorld._20 * objectSpaceMin.x;
			}

			if (objectToWorld._21 > 0.0f)
			{
				worldSpaceMin.z += objectToWorld._21 * objectSpaceMin.y;
				worldSpaceMax.z += objectToWorld._21 * objectSpaceMax.y;
			}
			else
			{
				worldSpaceMin.z += objectToWorld._21 * objectSpaceMax.y;
				worldSpaceMax.z += objectToWorld._21 * objectSpaceMin.y;
			}

			if (objectToWorld._22 > 0.0f)
			{
				worldSpaceMin.z += objectToWorld._22 * objectSpaceMin.z;
				worldSpaceMax.z += objectToWorld._22 * objectSpaceMax.z;
			}
			else
			{
				worldSpaceMin.z += objectToWorld._22 * objectSpaceMax.z;
				worldSpaceMax.z += objectToWorld._22 * objectSpaceMin.z;
			}
		}
	}
}