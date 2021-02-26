#pragma once
#include "CGContext/CGContext.h"
class Application
{
public:
	CGContext context;
	Application(int width, int height)
		:context(width,height){}
	virtual ~Application(){}

	virtual void program_step() = 0;
};

