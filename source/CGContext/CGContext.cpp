#include "CGContext/CGContext.h"
#include "Utilities/CG1Helper/CG1Helper.h"    // We need to speak with the helper during initialization.
#include <iostream>       // If we need to print something to the console...
#include "CGContext/CG.h"

// This method is usded to silence the unused, parameter warnings!
template<typename ... Args> void unused(const Args& ...){ }


//------------------------------------------------------------------------------
CGContext::CGContext(int width, int height)
	: m_frameBuffer(),
	  m_fragmentOps(*this,m_frameBuffer),
	  m_pointRasterizer(*this,m_fragmentOps),
	  m_stupidLineRasterizer(*this,m_fragmentOps),
	  m_bresenhamLineRasterizer(*this,m_fragmentOps),
	  m_triangleRasterizer(*this,m_fragmentOps),
	  m_primitiveOps(*this,
			 m_pointRasterizer,
			 m_stupidLineRasterizer,
			 m_bresenhamLineRasterizer,
			 m_triangleRasterizer),
	  m_vertexOps(*this,m_primitiveOps),
	  viewport(0,0,width,height)
{
	blendingIsEnabled
	                = depthTestIsEnabled
	                = backFaceCullingIsEnabled
	                = bresenhamIsEnabled
	                = false;
	// Default all internal states.

	clearColor = CGVec4(0,0,0,1);
	pointRadius = 0;

	polygonMode = CG_FILL;

	// allocate the framebuffer
	m_frameBuffer.allocate(width, height);
}

//------------------------------------------------------------------------------
CGContext::~CGContext()
{
}
//------------------------------------------------------------------------------
void CGContext::clear(unsigned int bitfield)
{
	if(bitfield & CG_COLOR_BUFFER_BIT)
		m_frameBuffer.colorBuffer.clear(clearColor);

	// U09 A1 b)
	if (bitfield & CG_DEPTH_BUFFER_BIT)
		m_frameBuffer.depthBuffer.clear();
}
//------------------------------------------------------------------------------
void CGContext::drawArrays(CGPrimitiveType mode, unsigned int first, unsigned int count)
{
	m_primitiveOps.setPrimitiveMode(mode);
	unsigned int last = first + count;
	for(unsigned int i = first ; i< last;i++)
		m_vertexOps.pullVertex(i);
}

//------------------------------------------------------------------------------
void CGContext::useProgram(const CGProgram& prog)
{
	currentProgram = &prog;
}

//------------------------------------------------------------------------------
void CGContext::readPixels(unsigned char *data) const
{
	// Remove this line, before implementing this method.
	unused(data);
	// ...
}
