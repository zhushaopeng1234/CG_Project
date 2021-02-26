#pragma once
#include "Applications/Application.h"
#include "ShaderPrograms/Program_passThrough.h"

class App_JumpingLine :public Application {

	int frame_width,frame_height;
	struct JumpingPoint
	{
		float dx;
		float dy;
		float x;
		float y;
	} a,b;

	void animateJumpingPoint(JumpingPoint& p);

	float position[2][3];	// x,y,z, x,y,z
	float color[2][4];	// r,g,b,a, r,g,b,a

	Program_passThrough prog;
public:
	App_JumpingLine(int w, int h);
	void program_step();

};

