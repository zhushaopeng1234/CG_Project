#pragma once

#include "CGContext/CGVector.h"

class CGContext;

class CGShape
{
    float* m_positions;
    float* m_normals;
    float* m_colors;
    float* m_texcoords;
    unsigned int m_vertexCount;

public:
	CGShape();
	CGShape(const CGShape& other);
	virtual ~CGShape();

	// creates object geometry
	virtual void create() = 0;

	// ray-object intersection in object space
    virtual float intersect(const CGVec4& ray_origin, const CGVec4& ray_direction) = 0;

	void draw(CGContext& context);

protected:
	void m_addTriangle(float x0, float y0, float z0, float nx0, float ny0, float nz0, float s0, float t0,
					   float x1, float y1, float z1, float nx1, float ny1, float nz1, float s1, float t1,
					   float x2, float y2, float z2, float nx2, float ny2, float nz2, float s2, float t2);

	void m_addTriangle(float x0, float y0, float z0, float nx0, float ny0, float nz0,
					   float x1, float y1, float z1, float nx1, float ny1, float nz1,
					   float x2, float y2, float z2, float nx2, float ny2, float nz2);

	void m_addQuad(float x0, float y0, float z0, float nx0, float ny0, float nz0, float s0, float t0,
				   float x1, float y1, float z1, float nx1, float ny1, float nz1, float s1, float t1,
				   float x2, float y2, float z2, float nx2, float ny2, float nz2, float s2, float t2,
				   float x3, float y3, float z3, float nx3, float ny3, float nz3, float s3, float t3);

	void m_addQuad(float x0, float y0, float z0, float nx0, float ny0, float nz0,
				   float x1, float y1, float z1, float nx1, float ny1, float nz1,
				   float x2, float y2, float z2, float nx2, float ny2, float nz2,
				   float x3, float y3, float z3, float nx3, float ny3, float nz3);

	void m_subdivideTriangle(int subdiv,
							 float x0, float y0, float z0,
							 float x1, float y1, float z1,
							 float x2, float y2, float z2);
};

class CGPlaneShape : public CGShape
{
public:
	virtual void create() override;
	virtual float intersect(const CGVec4& ray_origin, const CGVec4& ray_direction) override;
};

class CGCubeShape : public CGShape
{
public:
	virtual void create() override;
	virtual float intersect(const CGVec4& ray_origin, const CGVec4& ray_direction) override;
};

class CGSphereShape : public CGShape
{
public:
	virtual void create() override;
    virtual float intersect(const CGVec4& ray_origin, const CGVec4& ray_direction) override;
};


