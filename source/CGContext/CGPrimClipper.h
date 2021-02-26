#pragma once
#include "CG.h"

// Forwad declaration
struct CGVaryings;
//----------------------------------------------------------------------------
// Primitive clipper works in homogeneous clip space.
//----------------------------------------------------------------------------
class CGPrimClipper
{
public:
	/// Bit mask, each bit for one clip boundary
	static unsigned int clip_bits;

	/// Pointer to output vertices.
	static const CGVaryings* out_buf();

	/// A point clipper, it does actually culling
	static unsigned int clip_point(CGVaryings *vinBuf, int vinCnt);

	/// A line clipper
	/// Return the vertex count after clipping.
	static unsigned int clip_line(CGVaryings *vinBuf, int vinCnt);

	/// A polygon clipper
	/// Return the vertex count after clipping.
	static unsigned int clip_poly(CGVaryings *vinBuf, int vinCnt);

private:
	/// output buffer, it is also the internal working buffer
	static CGVaryings sm_vout_buf[CG_MAX_VERTICES_IN_PIPELINE];

	/// Liang-Barsky line-clipping
	static unsigned int clip_line_LB(CGVaryings *vinBuf, int vinCnt);

	/// Sutherland-Hodgman polygon-clipping
	static unsigned int clip_poly_SH(CGVaryings *vinBuf, int vinCnt);

	/// Sutherland-Hodgman polygon-clipping against single clipping plane
	/// voutBufSize is the maximal size of the voutBuf.
	/// Return the number of output vertices.
	static int clip_poly_SH_single(unsigned int clipBound,
	                               const CGVaryings *vinBuf,
	                               int nvin, CGVaryings *voutBuf);
};

