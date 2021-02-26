#pragma once
#include "Applications/Application.h"

#include "Utilities/CGQuadric.h"
#include "Utilities/Deer.h"
#include "ShaderPrograms/Program_perVertexLighting.h"
#include "ShaderPrograms/Program_modelViewProjection.h"




class App_Texturing :public Application
{
	Program_perVertexLighting prog_lit_tex;
	Program_modelViewProjection prog_unlit;
	CGQuadric sphere;
	CGTexture2D test_texture;
	CGTexture2D deer_texture;
	Deer deer;
	bool showDeer;

	const float verticesQuad[6 * 3] = { -1, 0, -1, -1, 0, 1, 1, 0, -1, 1, 0, -1, -1, 0, 1, 1, 0, 1 };
	const float normalsQuad[6 * 3] = { 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0 };
	const float colorsQuad[6 * 4] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	const float texcoordsQuad1[6 * 2] = { 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1 };
	const float texcoordsQuad2[6 * 2] = { 0, 0, 0, 2, 2, 0, 2, 0, 0, 2, 2, 2 };

public:
	App_Texturing(int w, int h);

	void program_step();


	void renderQuadric(const CGQuadric &quadric);
};
