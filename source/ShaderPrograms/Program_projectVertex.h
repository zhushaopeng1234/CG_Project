#pragma once
#include "CGContext/CGProgram.h"
#include "CGContext/CGProgramInterface.h"
class Program_projectVertex : public CGProgram
{
public:
	struct
	{
		// U07 A1b)
		// ...
		CGMatrix4x4 projectionMatrix;
	}
	uniform;
	Program_projectVertex()
	{

	}
	~Program_projectVertex(){}


	void vertex_shader(const CGVertexAttributes& in,
	                   CGVaryings& out) const
	{
		// U06 A5b)
		// ...
		//out.position = in.position;
		//out.position = in.position.operator/(-in.position.z);
		// U07 A1c)
		out.position = uniform.projectionMatrix.operator*(in.position);
		out.color = in.color;
	}

	void fragment_shader(const CGVaryings& in,
	                     CGVec4& out) const
	{
		out = in.color;
	}
};

