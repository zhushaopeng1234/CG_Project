#include "CGPrimitiveOperations.h"
#include "CGPrimClipper.h"
#include "CGContext.h"
#include "CGRasterizer.h"
#include "CGMath.h"

// This method is usded to silence the unused, parameter warnings!
template<typename ... Args> void unused( const Args& ...){ }

//------------------------------------------------------------------------------
void CGPrimitiveOperations::invoke_line_rasterization(const CGVaryings& a, const CGVaryings& b)
{
	if (m_context.bresenhamIsEnabled)
		m_bresenham_line_rasterizer.rasterize(a, b);
	else
		m_stupid_line_rasterizer.rasterize(a, b);
}

//------------------------------------------------------------------------------
bool CGPrimitiveOperations::backface_culling(const CGVaryings& a, const CGVaryings& b, const CGVaryings& c)
{
	//unused(a, b, c);
	//return true;

	//PRJ 5)a
	if (m_context.backFaceCullingIsEnabled ) {
		CGVec4 N = CGMath::cross((b.position - a.position), (c.position - a.position));
		CGVec4 r(0.0f, 0.0f, 1.0f);
		float N_z = CGMath::dot(N, r);
		return (N_z <= 0.0f) ? false : true;
	}
	else { 
		return true;
	}
}

//------------------------------------------------------------------------------
void CGPrimitiveOperations::invoke_triangle_rasterization(const CGVaryings& a, const CGVaryings& b, const CGVaryings& c)
{
	// TODO: Übung 04, Aufgabe 2a
	// Hinweis: Mit den drei Varyings, welche der Methode übergeben werden, können wir ein einfaches Gitternetz eines 
	//          Dreiecks zeichnen. Dazu müssen wir lediglich eine Linie zwischen den einzelnen Punkten zeichnen. Das
	//          zeichnen der Linien übernimmt die Methode `invoke_line_rasterization`.
	if (backface_culling(a, b, c) == false) return; //skip triangle
	if (m_context.polygonMode == CG_FILL) { // fill triangle
		m_triangle_rasterizer.rasterize(a, b, c);
	}
	else {
		invoke_line_rasterization(a, b);
		invoke_line_rasterization(b, c);
		invoke_line_rasterization(a, c);
	}
}

//------------------------------------------------------------------------------
CGPrimitiveOperations::CGPrimitiveOperations(CGContext& context, CGPointRasterizer& point_raster, CGLineRasterizer& line_raster, CGLineRasterizer& bresenham_raster, CGTriangleRasterizer& tri_raster) : 
	m_context(context), m_point_rasterizer(point_raster), m_stupid_line_rasterizer(line_raster), m_bresenham_line_rasterizer(bresenham_raster), m_triangle_rasterizer(tri_raster)
{
	m_num_vertices = 0;

	setPrimitiveMode(CG_POINTS);
}

//------------------------------------------------------------------------------
void CGPrimitiveOperations::setPrimitiveMode(CGPrimitiveType prim_mode)
{
	m_primitive_mode = prim_mode;

	switch (m_primitive_mode)
	{
	case CG_POINTS: m_required_vertices = 1; break;
	case CG_LINES: m_required_vertices = 2; break;
	case CG_TRIANGLES: m_required_vertices = 3; break;
	}
}

//------------------------------------------------------------------------------
void CGPrimitiveOperations::pushVertex(const CGVaryings& v)
{
	// primitive assembly
	m_vertex_varyings[m_num_vertices++] = v;

	if(m_num_vertices < m_required_vertices)
		return;
	
	//Uebung07 2)a
	clip_primitive();
	
	perspective_divide_primitives();
	viewport_transform_primitives();

	// We might have to rasterize more than one primitive.
	for(unsigned int i=0; i<m_num_vertices; i+=m_required_vertices) 
	{

		switch (m_primitive_mode)
		{
		case CG_POINTS: m_point_rasterizer.rasterize(m_vertex_varyings[i]); break;
		case CG_LINES: invoke_line_rasterization(m_vertex_varyings[i], m_vertex_varyings[i+1]); break;
		case CG_TRIANGLES: 
			if (backface_culling(m_vertex_varyings[i], m_vertex_varyings[i + 1], m_vertex_varyings[i + 2])) {
				invoke_triangle_rasterization(m_vertex_varyings[i], m_vertex_varyings[i + 1], m_vertex_varyings[i + 2]);
			}
			//break;
		}
	}

	// Purge vertex list.
	m_num_vertices = 0;
}

//------------------------------------------------------------------------------
void CGPrimitiveOperations::clip_primitive()
{
	int newPrimCount;

	switch (m_primitive_mode)
	{
	case CG_POINTS:
		m_num_vertices = CGPrimClipper::clip_point(m_vertex_varyings, (int)m_num_vertices); break;
	case CG_LINES:
		m_num_vertices = CGPrimClipper::clip_line(m_vertex_varyings, (int)m_num_vertices); break;
	case CG_TRIANGLES:
		// Clipped vertices need to be re-assembled into triangles
		newPrimCount = (int)CGPrimClipper::clip_poly(m_vertex_varyings, (int)m_num_vertices) - 2;
		m_num_vertices = 0; //reset the pipeline

		for (int i = 0; i < newPrimCount; ++i)
		{
			//as triangle fan
			m_vertex_varyings[m_num_vertices++] = CGPrimClipper::out_buf()[0];
			m_vertex_varyings[m_num_vertices++] = CGPrimClipper::out_buf()[i + 1];
			m_vertex_varyings[m_num_vertices++] = CGPrimClipper::out_buf()[i + 2];
		}

		break;
	}
}
//------------------------------------------------------------------------------
void CGPrimitiveOperations::perspective_divide_primitives()
{
	//for (unsigned int i = 0; i < m_num_vertices; i++)
	//{
	//}
	//Uebung07 1)c
	for (unsigned int i = 0; i < m_num_vertices; i++)
	{
		CGVec4& pos = m_vertex_varyings[i].position;
		pos.x = pos.x / pos.w;
		pos.y = pos.y / pos.w;
		pos.z = pos.z / pos.w;
		pos.w = 1.0f / pos.w;
	}
}
//------------------------------------------------------------------------------
void CGPrimitiveOperations::viewport_transform_primitives()
{
	//for (unsigned int i = 0; i < m_num_vertices; i++)
	//{
	//	CGVec4& pos = m_vertex_varyings[i].position;
	//}
	//Uebung 06 2)a
	float x_ratio = ((float)m_context.viewport.width() / 2.0f);
	float y_ratio = ((float)m_context.viewport.height() / 2.0f);
	for (unsigned int i = 0; i < m_num_vertices; i++)
	{
		CGVec4& pos = m_vertex_varyings[i].position; //get NDC
		pos.x = (pos.x + 1) * x_ratio + m_context.viewport.bottomLeft().x;
		pos.y = (pos.y + 1) * y_ratio + m_context.viewport.bottomLeft().y;
		pos.z = (pos.z + 1) / 2.0f; //[-1,1]->[0,1]
	}
}
