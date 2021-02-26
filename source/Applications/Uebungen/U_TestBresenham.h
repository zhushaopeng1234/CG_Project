#pragma once

#include "Applications/Application.h"
#include "ShaderPrograms/Program_passThrough.h"

class App_TestBresenham :public Application {

	Program_passThrough prog;

	const float vertexPosition_TestBresenham[32*3]={
		20,30,0, 20,40,0, 24,29,0, 28,38,0, 27,27,0, 34,34,0, 29,24,0, 38,28,0,
		30,20,0, 40,20,0, 29,16,0, 38,12,0, 27,13,0, 34,6,0, 24,11,0, 28,2,0,
		20,10,0, 20,0,0, 16,11,0, 12,2,0, 13,13,0, 6,6,0, 11,16,0, 2,12,0,
		10,20,0, 0,20,0, 11,24,0, 2,28,0, 13,27,0, 6,34,0, 16,29,0, 12,38,0
	};
	const float vertexColor_TestBresenham[32*4]={
		1,0,0,1, 1,1,1,1, 0,0,1,1, 1,1,0,1, 0,1,0,1, 1,0,1,1, 0,1,1,1, 1,0,0,1,
		1,0,0,1, 0,1,1,1, 1,0,1,1, 0,1,0,1, 1,1,0,1, 0,0,1,1, 1,1,1,1, 1,0,0,1,
		1,0,0,1, 0,0,1,1, 0,0,1,1, 0,1,0,1, 0,1,0,1, 0,1,1,1, 0,1,1,1, 1,0,0,1,
		1,0,0,1, 1,0,1,1, 1,0,1,1, 1,1,0,1, 1,1,0,1, 1,1,1,1, 1,1,1,1, 1,0,0,1
	};
public:
	App_TestBresenham(int w, int h);
	void program_step();

};

