#pragma once
#include "Applications/Application.h"
#include "ShaderPrograms/Program_passThrough.h"

class App_LineBenchmark :public Application {

	float position[2][3];	// x,y,z
	float color[2][4];	// r,g,b,a

	Program_passThrough prog;
public:
	App_LineBenchmark(int w, int h);
	void program_step();

};

