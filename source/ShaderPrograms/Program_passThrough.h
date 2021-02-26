#pragma once
#include "CGContext/CGProgram.h"
#include "CGContext/CGProgramInterface.h"
class Program_passThrough : public CGProgram
{
public:
	struct
	{

	}
	uniform;
	Program_passThrough()
	{

	}
	~Program_passThrough(){}

	void vertex_shader(const CGVertexAttributes& in,
	                   CGVaryings& out) const
	{

		out.position = in.position;
		out.color = in.color;
	}

	void fragment_shader(const CGVaryings& in,
	                     CGVec4& out) const
	{
		out = in.color;
	}
};


