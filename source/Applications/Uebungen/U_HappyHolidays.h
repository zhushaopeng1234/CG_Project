#pragma once
#include "Applications/Application.h"
#include "ShaderPrograms/Program_modelViewProjection.h"
#include "Utilities/Deer.h"
#include "Utilities/Snowflakes.h"
#define NUM_DEERS 6
#define NUM_SNOWFLAKES 512

class App_HappyHolidays:public Application {

	Program_modelViewProjection prog;

	float polar = 15.0f, azimuth = 0.0f;

	// the deer herd
	DeerHerd deers;
	// the snowflakes
	Snowflakes snowflakes;

	// ground and tree data
	const float verticesTrunk[3 * 3] = {-0.2f, 0.0f, 0.1f,
					    0.2f, 0.0f, 0.1f,
					    0.0f, 2.0f, 0.0f};
	const float colorsTrunk[3 * 4] = {0.5f, 0.25f, 0.0f, 1.0f,
					  0.5f, 0.25f, 0.0f, 1.0f,
					  0.5f, 0.25f, 0.0f, 1.0f};
	const float verticesLeafs[3 * 3] = {0.0f, 0.0f, 0.0f,
					    2.0f, 0.0f, 0.2f,
					    0.0f, 2.0f, 0.2f};
	const float colorsLeafs[3 * 4] = {1.0f, 1.0f, 1.0f, 1.0f,
					  0.0f, 0.8f, 0.0f, 1.0f,
					  0.0f, 0.5f, 0.0f, 1.0f};
	const float verticesGround[6 * 3] = {-10.0f, 0.0f,-10.0f,
					     10.0f, 0.0f, 10.0f,
					     10.0f, 0.0f,-10.0f,
					     -10.0f, 0.0f,-10.0f,
					     -10.0f, 0.0f, 10.0f,
					     10.0f, 0.0f, 10.0f};
	const float colorsGround[6 * 4] = {0.0f, 0.6f, 0.1f, 1.0f,
					   1.0f, 1.0f, 1.0f, 1.0f,
					   0.0f, 0.7f, 0.1f, 1.0f,
					   0.0f, 0.6f, 0.1f, 1.0f,
					   1.0f, 1.0f, 1.0f, 1.0f,
					   1.0f, 1.0f, 1.0f, 1.0f};

	void drawGround(const CGMatrix4x4& view);
	void drawTree(const CGMatrix4x4& view, const CGMatrix4x4 translation);
	void drawColoredRect(float r, float g, float b, float a);
	void renderDeers(const CGMatrix4x4& view);

public:
	App_HappyHolidays(int w, int h);
	void program_step();
};

