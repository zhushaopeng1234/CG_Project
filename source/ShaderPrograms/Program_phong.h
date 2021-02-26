#pragma once
#include <algorithm>
#include "CGContext/CGProgram.h"
#include "CGContext/CGProgramInterface.h"
#include "CGContext/CGMath.h"

// TODO: one project requirement is phong shading
// you might want to use Program_perVertexLighting
// (from after Uebung 10) as a basis and modify it
// for phong shading...

class Program_phong : public CGProgram
{
public:
	struct
	{
		//...
	}
	uniform;
	Program_phong()
	{
		//...
	}
	~Program_phong();
	void vertex_shader(const CGVertexAttributes& in,
	                   CGVaryings& out) const
	{
		// ...
	}
	void fragment_shader(const CGVaryings& in,
	                     CGVec4& out) const
	{
		// ...
	}
};

Program_phong::~Program_phong(){}
