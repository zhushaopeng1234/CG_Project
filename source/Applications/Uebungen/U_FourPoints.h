#pragma once
#include "Applications/Application.h"
#include "ShaderPrograms/Program_passThrough.h"

class App_FourPoints :public Application {

	int frame_width,frame_height;

	float position[4][3];	// x,y,z, x,y,z ,...
	float color[4][4];	// r,g,b,a, r,g,b,a, ...

	Program_passThrough prog;
public:
	App_FourPoints(int w, int h);
	void program_step();

};

