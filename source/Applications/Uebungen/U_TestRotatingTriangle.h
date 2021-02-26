#pragma once
#include "Applications/Application.h"
#include "ShaderPrograms/Program_passThrough.h"

class App_TestRotatingTriangle:public Application {

	Program_passThrough prog;

	const float vertexColor_TestRotTriangle[9*4] ={1,0,0,1, 0,0,1,1, 0,1,0,1,
	                                               1,0,0,1, 0,0,1,1, 0,1,0,1,
	                                               1,0,0,1, 0,0,1,1, 0,1,0,1};
public:
	App_TestRotatingTriangle(int w, int h);
	void program_step();

};

