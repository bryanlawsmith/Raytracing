#pragma once

namespace Raytracer
{
	struct Vertex
	{
		float m_Position[4];
		float m_Normal[4];
	};

	struct Triangle
	{
		Vertex m_Vertices[3];
	};
}