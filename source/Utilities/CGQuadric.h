#pragma once
#include "CGContext/CGVector.h"
#include "CGContext/CG.h"
//---------------------------------------------------------------------------
// CGQuadric-Class
// creates and holds polygonal (triangulated) discretizations
// of objects which can be described through quadratic equations.
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class CGQuadric
{
public:
	/// Empty constructor.
	CGQuadric();
	/// Copy constructor.
	CGQuadric(const CGQuadric &q);
	/// Destructor.
	~CGQuadric();
	/// Vertex count getter:
	unsigned int getVertexCount() const;
	/// Vertex positions getter:
	const float* getPositionArray() const;
	/// Vertex normals getter:
	const float* getNormalArray() const;
	/// Vertex colors getter:
	const float* getColorArray() const;
	/// Vertex texture coordinates getter:
	const float* getTexCoordArray() const;
	/// Set the color (before!) creating the object.
	void setStandardColor(const CGVec4& color);
	/// Create a sphere object.
	void createIcoSphere(int subdiv);

private:
	/// Internal method to add single trianglei (with texcoords).
	void m_addTriangle(float x0, float y0, float z0, float nx0, float ny0, float nz0, float s0, float t0,
			   float x1, float y1, float z1, float nx1, float ny1, float nz1, float s1, float t1,
			   float x2, float y2, float z2, float nx2, float ny2, float nz2, float s2, float t2);
	/// Internal method to add single triangle (without texcoords).
	void m_addTriangle(float x0, float y0, float z0, float nx0, float ny0, float nz0,
			   float x1, float y1, float z1, float nx1, float ny1, float nz1,
			   float x2, float y2, float z2, float nx2, float ny2, float nz2);

	/// Internal method to add single quad (with texcoords).
	void m_addQuad(	float x0, float y0, float z0, float nx0, float ny0, float nz0, float s0, float t0,
			float x1, float y1, float z1, float nx1, float ny1, float nz1, float s1, float t1,
			float x2, float y2, float z2, float nx2, float ny2, float nz2, float s2, float t2,
			float x3, float y3, float z3, float nx3, float ny3, float nz3, float s3, float t3);
	/// Internal method to add single quad (without texcoords).
	void m_addQuad(	float x0, float y0, float z0, float nx0, float ny0, float nz0,
			float x1, float y1, float z1, float nx1, float ny1, float nz1,
			float x2, float y2, float z2, float nx2, float ny2, float nz2,
			float x3, float y3, float z3, float nx3, float ny3, float nz3);

	/// Subdive triangle into 4 smaller ones.
	void m_subdivideTriangle(int subdiv,
				 float x0, float y0, float z0,
				 float x1, float y1, float z1,
				 float x2, float y2, float z2);

	/// Internal pointers to data.
	float *m_positions, *m_normals, *m_colors, *m_texcoords;

	/// Internal vertex count.
	unsigned int m_vertexCount;
	CGVec4 m_standardRGBA;
};
//---------------------------------------------------------------------------
