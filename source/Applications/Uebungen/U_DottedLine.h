#pragma once
#include "Applications/Application.h"
#include "ShaderPrograms/Program_passThrough.h"

class App_DottedLine : public Application {

	int frame_width, frame_height;

	float* position = nullptr;
	float* color = nullptr;
	int numSamples;

	Program_passThrough prog;

public:
	App_DottedLine(int w, int h);
	~App_DottedLine();
	void program_step();

};
