#pragma once
#include "CGProgramInterface.h"


class CGContext;
class CGPointRasterizer;
class CGLineRasterizer;
class CGTriangleRasterizer;;
class CGPrimitiveOperations
{
protected:
	CGContext& m_context;
	CGPointRasterizer& m_point_rasterizer;
	CGLineRasterizer& m_stupid_line_rasterizer;
	CGLineRasterizer& m_bresenham_line_rasterizer;
	CGTriangleRasterizer& m_triangle_rasterizer;


	CGVaryings m_vertex_varyings[CG_MAX_VERTICES_IN_PIPELINE];
	unsigned int m_num_vertices;

	CGPrimitiveType m_primitive_mode;
	unsigned int m_required_vertices;

	/// starts the rasterization of a line form a to b. The context's state
	void invoke_line_rasterization(const CGVaryings& a,
	                               const CGVaryings& b);

	/// test if triangle is to be rendered (if backFaceCulling capability is on)
	bool backface_culling(const CGVaryings &a,
	                      const CGVaryings &b,
	                      const CGVaryings &c);

	/// starts the rasterization of a trinalge a,b,c.
	void invoke_triangle_rasterization(const CGVaryings& a,
	                                   const CGVaryings& b,
	                                   const CGVaryings& c);
	/// clips the current primitive
	void clip_primitive();

	/// performs the perspective divide on all vertices of the current primitves
	void perspective_divide_primitives();

	/// performs the viewport transformation of all vertices of the
	/// current primitives
	void viewport_transform_primitives();
public:
	CGPrimitiveOperations( CGContext& context,
	                       CGPointRasterizer& point_raster,
	                       CGLineRasterizer& line_raster,
	                       CGLineRasterizer& bresenham_line_raster,
	                       CGTriangleRasterizer& triangle_raster);

	/// sets the primitive mode. This is called by the context when a draw
	/// call is issued.
	void setPrimitiveMode(CGPrimitiveType prim_mode);

	/// push a new vertex to the primitive operations stage
	void pushVertex(const CGVaryings& v);


};
