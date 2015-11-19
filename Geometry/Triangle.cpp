#include "Triangle.h"

namespace GeometryLib
{
	TriangleVertex::TriangleVertex()
	{
	}

	TriangleVertex::TriangleVertex(const vector4& position, 
								   const vector4& texCoord)
	{
		vector4_copy(m_Position, position);
		vector4_copy(m_TexCoord, texCoord);
	}

	TriangleVertex::TriangleVertex(const vector4& position, 
								   const vector4& texCoord, 
								   const vector4& normal) : 
		TriangleVertex(position, texCoord)
	{
		vector4_copy(m_Normal, normal);
	}

	TriangleVertex::TriangleVertex(const TriangleVertex& vertex)
	{
		vector4_copy(m_Position, vertex.m_Position);
		vector4_copy(m_TexCoord, vertex.m_TexCoord);
		vector4_copy(m_Normal, vertex.m_Normal);
	}

	TriangleVertex& TriangleVertex::operator=(const TriangleVertex& vertex)
	{
		vector4_copy(m_Position, vertex.m_Position);
		vector4_copy(m_TexCoord, vertex.m_TexCoord);
		vector4_copy(m_Normal, vertex.m_Normal);

		return *this;
	}

	Triangle::Triangle()
	{
	}

	Triangle::Triangle(const TriangleVertex& vertex0,
					   const TriangleVertex& vertex1,
					   const TriangleVertex& vertex2)
	{
		m_Vertices[0] = vertex0;
		m_Vertices[1] = vertex1; 
		m_Vertices[2] = vertex2;
	}

	Triangle::~Triangle()
	{
	}

	void Triangle::Subdivide(vector<Triangle>& outputVector)
	{
		vector4 p0p1Position;
		vector4 p0p1TexCoord;
		vector4_sub(m_Vertices[1].m_Position, m_Vertices[0].m_Position, p0p1Position);
		vector4_sub(m_Vertices[1].m_TexCoord, m_Vertices[0].m_TexCoord, p0p1TexCoord);

		vector4 p1p2Position;
		vector4 p1p2TexCoord;
		vector4_sub(m_Vertices[2].m_Position, m_Vertices[1].m_Position, p1p2Position);
		vector4_sub(m_Vertices[2].m_TexCoord, m_Vertices[1].m_TexCoord, p1p2TexCoord);

		vector4 p2p0Position;
		vector4 p2p0TexCoord;
		vector4_sub(m_Vertices[0].m_Position, m_Vertices[2].m_Position, p2p0Position);
		vector4_sub(m_Vertices[0].m_TexCoord, m_Vertices[2].m_TexCoord, p2p0TexCoord);

		// Calculate midpoints.
		vector4 midpoint01Position;
		vector4 midpoint01TexCoord;
		vector4_addScaledVector(m_Vertices[0].m_Position, p0p1Position, 0.5f, midpoint01Position);
		vector4_addScaledVector(m_Vertices[0].m_TexCoord, p0p1TexCoord, 0.5f, midpoint01TexCoord);

		vector4 midpoint12Position;
		vector4 midpoint12TexCoord;
		vector4_addScaledVector(m_Vertices[1].m_Position, p1p2Position, 0.5f, midpoint12Position);
		vector4_addScaledVector(m_Vertices[1].m_TexCoord, p1p2TexCoord, 0.5f, midpoint12TexCoord);

		vector4 midpoint20Position;
		vector4 midpoint20TexCoord;
		vector4_addScaledVector(m_Vertices[2].m_Position, p2p0Position, 0.5f, midpoint20Position);
		vector4_addScaledVector(m_Vertices[2].m_TexCoord, p2p0TexCoord, 0.5f, midpoint20TexCoord);

		// Generate new triangles.
		Triangle triangle0(TriangleVertex(m_Vertices[0].m_Position, m_Vertices[0].m_TexCoord),
						   TriangleVertex(midpoint01Position, midpoint01TexCoord),
						   TriangleVertex(midpoint20Position, midpoint20TexCoord));
		outputVector.push_back(triangle0);

		Triangle triangle1(TriangleVertex(midpoint01Position, midpoint01TexCoord),
						   TriangleVertex(m_Vertices[1].m_Position, m_Vertices[1].m_TexCoord),
						   TriangleVertex(midpoint12Position, midpoint12TexCoord));
		outputVector.push_back(triangle1);

		Triangle triangle2(TriangleVertex(m_Vertices[2].m_Position, m_Vertices[2].m_TexCoord),
						   TriangleVertex(midpoint20Position, midpoint20TexCoord),
						   TriangleVertex(midpoint12Position, midpoint12TexCoord));
		outputVector.push_back(triangle2);

		Triangle triangle3(TriangleVertex(midpoint01Position, midpoint01TexCoord),
						   TriangleVertex(midpoint12Position, midpoint12TexCoord),
						   TriangleVertex(midpoint20Position, midpoint20TexCoord));
		outputVector.push_back(triangle3);
	}
}