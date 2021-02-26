#pragma once
#include "Applications/Application.h"
#include "ShaderPrograms/Program_passThrough.h"

class App_AwesomeTriangle:public Application {

	Program_passThrough prog;

	float vertices[3 * 3 * 20], colors[3 * 4 * 20];
	float step = 0.0f;
public:
	App_AwesomeTriangle(int w, int h);
	void program_step();

};

