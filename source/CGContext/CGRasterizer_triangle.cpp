#include "CGRasterizer.h"
#include "CGProgramInterface.h"
#include "CGFragmentOperations.h"
#include "CGContext.h"
#include "CGMath.h"
#include <algorithm>
//------------------------------------------------------------------------------
void CGTriangleRasterizer::rasterize(const CGVaryings& A, const CGVaryings& B, const CGVaryings& C)
{
	// shortcuts for position of each input vertex
	const CGVec4& a = A.position;
	const CGVec4& b = B.position;
	const CGVec4& c = C.position;
	CGFragmentData fragment;

	// TODO: Uebung 05, Aufgaben 1b, 2b, 3


	


	float xmax = std::max(std::max(a.x, b.x), c.x);
	float xmin = std::min(std::min(a.x, b.x), c.x);
	float ymax = std::max(std::max(a.y, b.y), c.y);
	float ymin = std::min(std::min(a.y, b.y), c.y);


	//    int w = m_context.m_frameBuffer.getWidth();
	//    int h = m_context.m_frameBuffer.getHeight();

	float detABC = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);

	CGVec2 p;
	for (int Px = xmin; Px <= xmax; Px++) {
		for (int Py = ymin; Py <= ymax; Py++) {
			p.set((float)Px + 0.5F, (float)Py + 0.5F);
			float alphaN = (b.x - p.x) * (c.y - p.y) - (b.y - p.y) * (c.x - p.x);
			float betaN = (c.x - p.x) * (a.y - p.y) - (c.y - p.y) * (a.x - p.x);
			float gammaN = (a.x - p.x) * (b.y - p.y) - (a.y - p.y) * (b.x - p.x);
			float alpha = alphaN / detABC;
			float beta = betaN / detABC;
			float gamma = gammaN / detABC;

			if (alpha >= 0 && beta >= 0 && gamma >= 0) { 
				fragment.coordinates.set(p.x, p.y);
				fragment.set(A, B, C, alpha, beta, gamma);
				m_frag_ops.push_fragment(fragment);
			}
		}
	}

	m_frag_ops.flush_fragments();
}
