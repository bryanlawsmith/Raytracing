#include "RayTriangle.h"
#include <iostream>

namespace GeometryLib
{

	bool RayTriangleIntersection(const ray& intersectionRay,
								 const Triangle& triangle,
								 float& t,
								 float& u,
								 float& v)
	{
		// Moller-Trumbore algorithm.
		const vector4& rayDirection = intersectionRay.getDirection();
		const vector4& rayPosition = intersectionRay.getPosition();

		// Calculate edges.
		vector4 edge1; 
		vector4_sub(triangle.m_Vertices[2].m_Position, triangle.m_Vertices[0].m_Position, edge1);

		vector4 edge2;
		vector4_sub(triangle.m_Vertices[1].m_Position, triangle.m_Vertices[0].m_Position, edge2);

		// Begin calculating determinant, also used to calculate u coordinate.
		vector4 P;
		vector4_crossProduct(rayDirection, edge2, P);

		// Calculate determinant. If the determinant is near zero, the ray lies in the plane of the triangle.
		float determinant = vector4_dotProduct(edge1, P);

		// Perform back face culling, the triangle is facing away from the ray if the determinant is less than 0.
		if (determinant < 0.0f)
			return false;

		// Calculate distance from the first vertex to the ray origin.
		vector4 T;
		vector4_sub(rayPosition, triangle.m_Vertices[0].m_Position, T);

		// Calculate U coordinate and test bounds.
		float U = vector4_dotProduct(P, T);
		if (U < 0.0f || U > determinant)
			return false;

		vector4 Q;
		vector4_crossProduct(T, edge1, Q);

		// Calculate V coordinate and test bounds.
		float V = vector4_dotProduct(Q, rayDirection);
		if (V < 0.0f || U + V > determinant)
			return false;

		float invDeterminant = 1.0f / determinant;
		t = vector4_dotProduct(edge2, Q) * invDeterminant;
		u = U * invDeterminant;
		v = V * invDeterminant;

		return true;
	}

}
