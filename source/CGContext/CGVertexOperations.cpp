#include "CGVertexOperations.h"
#include "CGContext.h"
#include "CGProgram.h"
//------------------------------------------------------------------------------
CGVertexOperations::CGVertexOperations(CGContext &context, CGPrimitiveOperations &prim_ops)
	:m_context(context),m_prim_ops(prim_ops)
{

}
//------------------------------------------------------------------------------
void CGVertexOperations::pullVertex(const unsigned int vertexIndex)
{

	if(m_context.vertexAttributePointer.position) {
		const float *pos=m_context.vertexAttributePointer.position+3*vertexIndex;
		m_vertex_attribute.position.set(pos[0], pos[1], pos[2], 1.0f);
	} else {
		m_vertex_attribute.position.set(0.0f, 0.0f, 0.0f, 1.0f);
	}

	if(m_context.vertexAttributePointer.normal) {
		const float *nrm=m_context.vertexAttributePointer.normal+3*vertexIndex;
		m_vertex_attribute.normal.set(nrm[0], nrm[1], nrm[2], 0.0f);
	} else {
		m_vertex_attribute.normal.set(0.0f, 0.0f, 1.0f, 0.0f);
	}

	if(m_context.vertexAttributePointer.color) {
		const float *col=m_context.vertexAttributePointer.color+4*vertexIndex;
		m_vertex_attribute.color.set(col[0], col[1], col[2], col[3]);
	} else {
		m_vertex_attribute.color.set(0.0f, 0.0f, 0.0f, 1.0f);
	}

	if(m_context.vertexAttributePointer.texcoord) {
		const float *tex=m_context.vertexAttributePointer.texcoord+2*vertexIndex;
		m_vertex_attribute.texcoord.set(tex[0], tex[1], 0.0f, 1.0f);
	} else {
		m_vertex_attribute.texcoord.set(0.0f, 0.0f, 0.0f, 1.0f);
	}

	CGVaryings v;
	m_context.currentProgram->vertex_shader(m_vertex_attribute,v);
	m_prim_ops.pushVertex(v);
}
