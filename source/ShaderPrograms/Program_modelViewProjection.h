#pragma once
#include "CGContext/CGProgram.h"
#include "CGContext/CGProgramInterface.h"
class Program_modelViewProjection : public CGProgram
{
public:
	struct
	{
		CGMatrix4x4 projectionMatrix;
		CGMatrix4x4 modelViewMatrix;
	}
	uniform;
	Program_modelViewProjection()
	{

	}
	~Program_modelViewProjection(){}

	void vertex_shader(const CGVertexAttributes& in, CGVaryings& out) const
	{
		//out.position = in.position;
		// U08 A2
		// ...
		out.position = uniform.projectionMatrix * uniform.modelViewMatrix * (in.position);
		out.color = in.color;
	}

	void fragment_shader(const CGVaryings& in, CGVec4& out) const
	{
		out = in.color;
	}
};


