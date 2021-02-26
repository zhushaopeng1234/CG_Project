#pragma once
#include "CGProgramInterface.h"
class CGContext;
class CGPrimitiveOperations;

class CGVertexOperations
{
protected:
	CGContext& m_context;
	CGPrimitiveOperations& m_prim_ops;
	CGVertexAttributes m_vertex_attribute;
public:
	CGVertexOperations(CGContext& context,
			   CGPrimitiveOperations& prim_ops);

	/// pullVertex inits the processing of one vertex.
	void pullVertex(const unsigned int vertexID);
};
