#pragma once
#include "Applications/Application.h"
#include "ShaderPrograms/Program_projectVertex.h"

class App_TestProjection:public Application {

	Program_projectVertex prog;

	bool wire;

	const float verticesGround[6 * 3]=
	{-0.5f,-0.5f,1.f,  0.5f,-0.5f, 1.f,   0.5f,-0.5f,-1.f,
	 -0.5f,-0.5f,1.f,  0.5f,-0.5f,-1.f,  -0.5f,-0.5f,-1.f};
	const float verticesCubeBase[12 * 3 * 3] = {
		0.f,0.f,0.f,1.f,1.f,0.f,1.f,0.f,0.f,0.f,0.f,0.f,0.f,1.f,0.f,1.f,1.f,0.f, // BACK
		1.f,0.f,0.f,1.f,0.f,1.f,0.f,0.f,1.f,0.f,0.f,1.f,0.f,0.f,0.f,1.f,0.f,0.f, // BOTTOM
		1.f,0.f,1.f,1.f,0.f,0.f,1.f,1.f,0.f,1.f,0.f,1.f,1.f,1.f,0.f,1.f,1.f,1.f, // RIGHT
		0.f,1.f,0.f,0.f,0.f,0.f,0.f,0.f,1.f,0.f,0.f,1.f,0.f,1.f,1.f,0.f,1.f,0.f, // LEFT
		0.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,0.f,0.f,1.f,1.f,1.f,1.f,0.f,0.f,1.f,0.f, // TOP
		0.f,0.f,1.f,1.f,0.f,1.f,1.f,1.f,1.f,0.f,0.f,1.f,1.f,1.f,1.f,0.f,1.f,1.f};// FRONT
	float colorsGround[6 * 4];
	float verticesCube[12 * 3 * 3];
	float colorsCube[12 * 3 * 4];

public:
	App_TestProjection(int w, int h);
	void program_step();

};

