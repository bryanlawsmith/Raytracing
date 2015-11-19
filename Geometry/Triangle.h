#pragma once

#include "GeometryLibCommon.h"
#include <MathLib.h>
#include <vector>

using namespace MathLib;
using namespace std;

namespace GeometryLib
{

	class TriangleVertex
	{
	public:

		vector4 m_Position;
		vector4 m_TexCoord;
		vector4 m_Normal;

		TriangleVertex();

		TriangleVertex(const vector4& position,
						const vector4& texCoord);

		TriangleVertex(const vector4& position,
						const vector4& texCoord,
						const vector4& normal);

		TriangleVertex(const TriangleVertex& vertex);

		TriangleVertex& operator=(const TriangleVertex& vertex);
	};

	class Triangle
	{
	public:

		TriangleVertex m_Vertices[3];

		Triangle();

		Triangle(const TriangleVertex& vertex0,
				 const TriangleVertex& vertex1,
				 const TriangleVertex& vertex2);

		~Triangle();

		/// <summary>
		/// Subdivides this triangle using midpoint subdivision and outputs the resulting four
		/// triangles in the output vector.
		/// </summary>
		void Subdivide(vector<Triangle>& outputVector);
	};

}
