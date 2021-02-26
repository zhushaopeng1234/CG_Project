#include "CGShape.h"

#include <cstdlib>
#include <cstring>
#include <utility>

#include "CGContext/CGMath.h"
#include "CGContext.h"

CGShape::CGShape()
{
	m_positions = m_normals = m_colors = m_texcoords = nullptr;
	m_vertexCount = 0;
}

CGShape::CGShape(const CGShape& q)
{
	m_vertexCount = q.m_vertexCount;

	m_positions = (float*)malloc(sizeof(float) * 3 * m_vertexCount);
	memcpy(m_positions, q.m_positions, sizeof(float) * 3 * m_vertexCount);

	m_normals = (float*)malloc(sizeof(float) * 3 * m_vertexCount);
	memcpy(m_normals, q.m_normals, sizeof(float) * 3 * m_vertexCount);

	m_colors = (float*)malloc(sizeof(float) * 4 * m_vertexCount);
	memcpy(m_colors, q.m_colors, sizeof(float) * 4 * m_vertexCount);

	m_texcoords = (float*)malloc(sizeof(float) * 2 * m_vertexCount);
	memcpy(m_texcoords, q.m_texcoords, sizeof(float) * 2 * m_vertexCount);
}

CGShape::~CGShape()
{
	free(m_positions);
	free(m_normals);
	free(m_colors);
	free(m_texcoords);
}

void CGShape::draw(CGContext& context)
{
	context.vertexAttributePointer.position = m_positions;
	context.vertexAttributePointer.normal = m_normals;
	context.vertexAttributePointer.color = m_colors;
	context.vertexAttributePointer.texcoord = m_texcoords;
	context.drawArrays(CG_TRIANGLES, 0, m_vertexCount);
	context.vertexAttributePointer.position = nullptr;
	context.vertexAttributePointer.normal = nullptr;
	context.vertexAttributePointer.color = nullptr;
	context.vertexAttributePointer.texcoord = nullptr;
}

void CGShape::m_addTriangle(float x0, float y0, float z0, float nx0, float ny0, float nz0, float s0, float t0,
							float x1, float y1, float z1, float nx1, float ny1, float nz1, float s1, float t1,
							float x2, float y2, float z2, float nx2, float ny2, float nz2, float s2, float t2)
{
	static const CGVec4 vertex_color(1.0f);

	// Increase array storage by 3 vertices.
	m_positions = (float*)realloc(m_positions, sizeof(float) * 3 * (m_vertexCount + 3));
	m_normals = (float*)realloc(m_normals, sizeof(float) * 3 * (m_vertexCount + 3));
	m_colors = (float*)realloc(m_colors, sizeof(float) * 4 * (m_vertexCount + 3));
	m_texcoords = (float*)realloc(m_texcoords, sizeof(float) * 2 * (m_vertexCount + 3));

	// Get the next free array position.
	float* p = m_positions + 3 * m_vertexCount,
		* n = m_normals + 3 * m_vertexCount,
		* c = m_colors + 4 * m_vertexCount,
		* t = m_texcoords + 2 * m_vertexCount;

	// Set the respective values.
#define SET2(p,x,y)     { (p)[0]=x; (p)[1]=y;                     }
#define SET3(p,x,y,z)   { (p)[0]=x; (p)[1]=y; (p)[2]=z;           }
	SET3(p + 0, x0, y0, z0) SET3(n + 0, nx0, ny0, nz0) SET2(t + 0, s0, t0) memcpy(c + 0, vertex_color.getDataPtr(), sizeof(float) * 4);
	SET3(p + 3, x1, y1, z1) SET3(n + 3, nx1, ny1, nz1) SET2(t + 2, s1, t1) memcpy(c + 4, vertex_color.getDataPtr(), sizeof(float) * 4);
	SET3(p + 6, x2, y2, z2) SET3(n + 6, nx2, ny2, nz2) SET2(t + 4, s2, t2) memcpy(c + 8, vertex_color.getDataPtr(), sizeof(float) * 4);
#undef SET2
#undef SET3

	// Increase internal vertex count.
	m_vertexCount += 3;
}

void CGShape::m_addTriangle(float x0, float y0, float z0, float nx0, float ny0, float nz0,
							float x1, float y1, float z1, float nx1, float ny1, float nz1,
							float x2, float y2, float z2, float nx2, float ny2, float nz2)
{
	m_addTriangle(x0, y0, z0, nx0, ny0, nz0, 0.0f, 0.0f,
				  x1, y1, z1, nx1, ny1, nz1, 0.0f, 0.0f,
				  x2, y2, z2, nx2, ny2, nz2, 0.0f, 0.0f);
}

void CGShape::m_addQuad(float x0, float y0, float z0, float nx0, float ny0, float nz0, float s0, float t0,
						float x1, float y1, float z1, float nx1, float ny1, float nz1, float s1, float t1,
						float x2, float y2, float z2, float nx2, float ny2, float nz2, float s2, float t2,
						float x3, float y3, float z3, float nx3, float ny3, float nz3, float s3, float t3)
{
	m_addTriangle(x0, y0, z0, nx0, ny0, nz0, s0, t0,
				  x1, y1, z1, nx1, ny1, nz1, s1, t1,
				  x2, y2, z2, nx2, ny2, nz2, s2, t2);
	m_addTriangle(x0, y0, z0, nx0, ny0, nz0, s0, t0,
				  x2, y2, z2, nx2, ny2, nz2, s2, t2,
				  x3, y3, z3, nx3, ny3, nz3, s3, t3);
}

void CGShape::m_addQuad(float x0, float y0, float z0, float nx0, float ny0, float nz0,
						float x1, float y1, float z1, float nx1, float ny1, float nz1,
						float x2, float y2, float z2, float nx2, float ny2, float nz2,
						float x3, float y3, float z3, float nx3, float ny3, float nz3)
{
	m_addQuad(x0, y0, z0, nx0, ny0, nz0, 0.0f, 0.0f,
			  x1, y1, z1, nx1, ny1, nz1, 0.0f, 0.0f,
			  x2, y2, z2, nx2, ny2, nz2, 0.0f, 0.0f,
			  x3, y3, z3, nx3, ny3, nz3, 0.0f, 0.0f);
}

void CGShape::m_subdivideTriangle(int subdiv,
								  float x0, float y0, float z0,
								  float x1, float y1, float z1,
								  float x2, float y2, float z2)
{
	if (subdiv > 0) {
		//subdivide further
		--subdiv;
		float x3, y3, z3, x4, y4, z4, x5, y5, z5;
		//1. calculate new positions
		x3 = 0.5f * x0 + 0.5f * x1;    //            v2                //
		y3 = 0.5f * y0 + 0.5f * y1;    //           /  \               //
		z3 = 0.5f * z0 + 0.5f * z1;    //          /    \              //
		//         /      \             //
		x4 = 0.5f * x1 + 0.5f * x2;    //        /        \            //
		y4 = 0.5f * y1 + 0.5f * y2;    //       v5---------v4          //
		z4 = 0.5f * z1 + 0.5f * z2;    //      / \        / \          //
		//     /   \      /   \         //
		x5 = 0.5f * x2 + 0.5f * x0;    //    /     \    /     \        //
		y5 = 0.5f * y2 + 0.5f * y0;    //   /       \  /       \       //
		z5 = 0.5f * z2 + 0.5f * z0;    // v0---------v3---------v1     //
		//2. normalize them
		float l;
		l = sqrtf(x3 * x3 + y3 * y3 + z3 * z3);
		x3 /= l; y3 /= l; z3 /= l;
		l = sqrtf(x4 * x4 + y4 * y4 + z4 * z4);
		x4 /= l; y4 /= l; z4 /= l;
		l = sqrtf(x5 * x5 + y5 * y5 + z5 * z5);
		x5 /= l; y5 /= l; z5 /= l;
		m_subdivideTriangle(subdiv,
							x0, y0, z0,
							x3, y3, z3,
							x5, y5, z5);
		m_subdivideTriangle(subdiv,
							x3, y3, z3,
							x1, y1, z1,
							x4, y4, z4);
		m_subdivideTriangle(subdiv,
							x5, y5, z5,
							x3, y3, z3,
							x4, y4, z4);
		m_subdivideTriangle(subdiv,
							x2, y2, z2,
							x5, y5, z5,
							x4, y4, z4);
	}
	else {
		m_addTriangle(x0, y0, z0, x0, y0, z0,
					  x1, y1, z1, x1, y1, z1,
					  x2, y2, z2, x2, y2, z2);
	}
}

void CGPlaneShape::create()
{
	m_addQuad(
		0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 2.0f, 2.0f,
		-0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 2.0f,
		-0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 2.0f, 0.0f
	);
}

float CGPlaneShape::intersect(const CGVec4& ray_origin, const CGVec4& ray_direction)
{
	if (abs(ray_direction.y) > 1e-6f)
	{
		const auto t = -ray_origin.y / ray_direction.y;
		if (t >= 0.0f)
		{
			const auto p = ray_origin + t * ray_direction;
			if (p.x >= -0.5f && p.x <= 0.5f && p.z >= -0.5f && p.z <= 0.5f)
				return t;
		}
	}

	return -1.0f;
}

void CGCubeShape::create()
{
	m_addQuad( // top
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f
	);
	m_addQuad( // bottom
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f
	);
	m_addQuad( // front
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f
	);
	m_addQuad( // back
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f
	);
	m_addQuad( // left
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f
	);
	m_addQuad( //right
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f
	);
}

float CGCubeShape::intersect(const CGVec4& ray_origin, const CGVec4& ray_direction)
{
	static const float min = -0.5f;
	static const float max = 0.5f;
	float t_near = -1e37f;
	float t_far = 1e37f;

	for (int i = 0; i < 3; ++i)
	{
		if (ray_direction[i])
		{
			auto t1 = (min - ray_origin[i]) / ray_direction[i];
			auto t2 = (max - ray_origin[i]) / ray_direction[i];

			if (t1 > t2)
				std::swap(t1, t2);

			if (t1 > t_near)
				t_near = t1;

			if (t2 < t_far)
				t_far = t2;

			if (t_far < 0.0f || t_near > t_far)
				return -1.0f;
		}
		else
		{
			if (ray_origin[i] < min || ray_origin[i] > max)
				return -1.0f;
		}
	}

	return t_near;
}

void CGSphereShape::create()
{
	static const int subdiv = 2;
	m_subdivideTriangle(subdiv,
						1, 0, 0,
						0, 0, -1,
						0, 1, 0);
	m_subdivideTriangle(subdiv,
						0, 0, -1,
						-1, 0, 0,
						0, 1, 0);
	m_subdivideTriangle(subdiv,
						-1, 0, 0,
						0, 0, 1,
						0, 1, 0);
	m_subdivideTriangle(subdiv,
						0, 0, 1,
						1, 0, 0,
						0, 1, 0);
	m_subdivideTriangle(subdiv,
						0, 0, -1,
						1, 0, 0,
						0, -1, 0);
	m_subdivideTriangle(subdiv,
						-1, 0, 0,
						0, 0, -1,
						0, -1, 0);
	m_subdivideTriangle(subdiv,
						0, 0, 1,
						-1, 0, 0,
						0, -1, 0);
	m_subdivideTriangle(subdiv,
						1, 0, 0,
						0, 0, 1,
						0, -1, 0);
}

float CGSphereShape::intersect(const CGVec4& ray_origin, const CGVec4& ray_direction)
{
	const float a = ray_direction.x * ray_direction.x + ray_direction.y * ray_direction.y + ray_direction.z * ray_direction.z;
	const float b = 2.0f * (ray_origin.x * ray_direction.x + ray_origin.y * ray_direction.y + ray_origin.z * ray_direction.z);
	const float c = ray_origin.x * ray_origin.x + ray_origin.y * ray_origin.y + ray_origin.z * ray_origin.z - 1.0f;
	const float d = b * b - 4.0f * a * c;

	if (d < 0.0f)
		return -1.0f;

	const float sqrt_d = sqrt(d);
	float t1 = (-b + sqrt(d)) / (2.0f * a);
	float t2 = (-b - sqrt(d)) / (2.0f * a);

	if (t1 > t2)
		std::swap(t1, t2);

	if (t1 >= 0.0f)
		return t1;

	return -1.0f;
}
