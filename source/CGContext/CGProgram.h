#pragma once
#include "CGVector.h"
struct CGVaryings;
struct CGVertexAttributes;


class CGProgram
{
public:
	CGProgram() {}
	virtual ~CGProgram(){}
	/// This will be called during the vertex processing stage.
	virtual void vertex_shader(const CGVertexAttributes& in,
	                           CGVaryings& out) const = 0;

	/// This will be called during the fragment processing stage.
	virtual void fragment_shader(const CGVaryings& in,
	                             CGVec4& out) const = 0;
};





