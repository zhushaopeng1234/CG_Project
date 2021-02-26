#pragma once
#include "CGContext/CGProgram.h"
#include "CGContext/CGProgramInterface.h"
class Program_testVertex : public CGProgram
{
public:
	struct
	{

	}
	uniform;
	Program_testVertex()
	{

	}
	~Program_testVertex(){}

	void vertex_shader(const CGVertexAttributes& in,
	                   CGVaryings& out) const
	{
		// U06 A4a)
		// ...
		//out.position = in.position;
		//out.color = in.color;
		out.position = 0.5f * in.position;
		out.color = in.color;
	}

	void fragment_shader(const CGVaryings& in,
	                     CGVec4& out) const
	{
		out = in.color;
	}
};
