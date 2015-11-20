#version 430

#define EPSILON 0.00001

/// Set the number of work items in the work group.
layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

/// Textures.
layout (binding = 0, rgba32f) uniform writeonly image2D texture;

uniform mat4 u_ScreenCoordTransform;
uniform mat4 u_RotationTransform;
uniform vec4 u_CameraPosition;

uniform int u_NumSceneTriangles;

struct Ray
{
	vec3 m_Position;
	vec3 m_Direction;
};

struct Sphere
{
	vec3 m_Position;
	float m_Radius;
	
	vec3 m_Color;
};

struct Vertex
{
	vec4 m_Position;
	vec4 m_Normal;
};

struct Triangle
{
	/// <summary>
	/// NOTE: Vertices are listed in clockwise order!
	/// </summary>
	Vertex m_Vertices[3];
};

layout (std430, row_major, binding = 1)  
buffer ssbo_Triangles 
{
	Triangle triangles[];
} ssbo;

bool RaySphereIntersection(in Ray ray, in Sphere sphere, out float t)
{
	// Check if the sphere intersects the ray.
	vec3 rayToSphere = sphere.m_Position - ray.m_Position;
	
	float  direction = dot(rayToSphere, ray.m_Direction);
	if (direction < EPSILON)
		return false;
	
	// Calculate projection.
	vec3 projectionVector = ray.m_Direction * (direction / 1.0); // 1.0 because rayDirection is a unit vector.
	
	vec3 sphereCenterToProjectionVector = (ray.m_Position + projectionVector) - sphere.m_Position;
	
	float distanceToProjectionPoint = length(sphereCenterToProjectionVector);
	
	if (distanceToProjectionPoint > sphere.m_Radius)
		return false;
	
	// Calculate the closest point.
	t = length(projectionVector) - sqrt((sphere.m_Radius * sphere.m_Radius) - (distanceToProjectionPoint * distanceToProjectionPoint));
	
	return true;
}

/// <summary>
/// Intersects a ray with a given triangle. Uses the Moller-Trumbore algorithm.
/// The results vector contains the results <t, u, v> where u and v are the barycentric 
/// coordinates of the intersection point.
/// </summary>
bool RayTriangleIntersection(in Ray ray, in Triangle triangle, out vec3 results)
{
	// Calculate edges.
	vec3 edge1 = triangle.m_Vertices[2].m_Position.xyz - triangle.m_Vertices[0].m_Position.xyz;
	vec3 edge2 = triangle.m_Vertices[1].m_Position.xyz - triangle.m_Vertices[0].m_Position.xyz;
	
	// Begin calculating determinant, also used to calculate U parameter.
	vec3 P = cross(ray.m_Direction, edge2);
	
	// Calculate determinant. If the determinant is near zero, the ray lies in the plane of the triangle.
	float determinant = dot(edge1, P);
	
	// Perform back face culling, the triangle is facing away from the ray if the determinant is less than 0.
	if (determinant < 0.0)
		return false;
	
	// Calculate distance from the first vertex to the ray origin.
	vec3 T = ray.m_Position - triangle.m_Vertices[0].m_Position.xyz;
	
	// Calculate U parameter and test bounds.
	float U = dot(P, T);
	if (U < 0.0 || U > determinant)
		return false;
	
	vec3 Q = cross(T, edge1);
	
	//Calculate V parameter and test bounds.
	float V  = dot(Q, ray.m_Direction);
	if (V < 0.0 || U + V > determinant)
		return false;
	
	// Calculate T, scale parameters.
	float t = dot(edge2, Q);
	results.xyz = vec3(t, U, V);
	results /= determinant;
	
	return true;
}

void main()
{
	// Acquire the coordinates of the pixel to process.
	ivec2 texelCoordinate = ivec2(gl_GlobalInvocationID.xy);
	
	vec4 screenRay = vec4(texelCoordinate.x, 720 - texelCoordinate.y, 0.0, 1.0);		// Need to flip y here.
	screenRay = u_ScreenCoordTransform * screenRay;
	
	// Set to vector for the rotation transform.
	screenRay.w = 0.0f;		
	screenRay = u_RotationTransform * screenRay;
	screenRay.xyz = normalize(screenRay).xyz;
	
	Ray ray;
	ray.m_Position = u_CameraPosition.xyz;
	ray.m_Direction = screenRay.xyz;
	
	float t = 999999;		// Would really love a FLT_MAX constant guys.
	float u = 0;
	float v = 0;
	int intersectedTriangleIndex = -1;
	for (int i = 0; i < u_NumSceneTriangles; i++)
	{		
		vec3 result;
		
		Triangle currentTriangle;	
		currentTriangle.m_Vertices[0].m_Position = ssbo.triangles[i].m_Vertices[0].m_Position;
		currentTriangle.m_Vertices[1].m_Position = ssbo.triangles[i].m_Vertices[1].m_Position;
		currentTriangle.m_Vertices[2].m_Position = ssbo.triangles[i].m_Vertices[2].m_Position;
		
		if (RayTriangleIntersection(ray, currentTriangle, result))
		{
			
			if (result.x + EPSILON < t && result.x > 0.0)
			{
				t = result.x;
				u = result.y;
				v = result.z;
				intersectedTriangleIndex = i;
			}
		}
	}
	
	if (intersectedTriangleIndex >= 0)
	{
		// Shade triangle.
		vec3 v0 = ssbo.triangles[intersectedTriangleIndex].m_Vertices[0].m_Position.xyz;
		vec3 v1 = ssbo.triangles[intersectedTriangleIndex].m_Vertices[1].m_Position.xyz;
		vec3 v2 = ssbo.triangles[intersectedTriangleIndex].m_Vertices[2].m_Position.xyz;
		
		vec3 n0 = ssbo.triangles[intersectedTriangleIndex].m_Vertices[0].m_Normal.xyz;
		vec3 n1 = ssbo.triangles[intersectedTriangleIndex].m_Vertices[1].m_Normal.xyz;
		vec3 n2 = ssbo.triangles[intersectedTriangleIndex].m_Vertices[2].m_Normal.xyz;
		
		vec3 interpolatedNormal = ((1 - u - v) * n0) + (v * n1) + (u * n2);
		interpolatedNormal = normalize(interpolatedNormal);
		
		// Calculate shading.
		float diffuseContribution = clamp(dot(interpolatedNormal, vec3(0.0, 1.0, 0.0)), 0.0, 1.0);
		diffuseContribution += 0.1;
		
		imageStore(texture, texelCoordinate, vec4(diffuseContribution, diffuseContribution, diffuseContribution, 1.0));
	}
	else
	{
		imageStore(texture, texelCoordinate, vec4(0.0, 0.0, 0.0, 0.0));
	}
}
