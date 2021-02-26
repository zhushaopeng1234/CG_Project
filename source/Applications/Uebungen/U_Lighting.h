#pragma once
#include "Applications/Application.h"
#include "Utilities/CGQuadric.h"
#include "Utilities/Deer.h"

#include "ShaderPrograms/Program_perVertexLighting.h"
#include "ShaderPrograms/Program_modelViewProjection.h"




class App_Lighting :public Application
{
	Program_perVertexLighting prog_lit;
	Program_modelViewProjection prog_unlit;
	CGQuadric sphere1;
	CGQuadric sphere2;
	CGTexture2D test_texture;
	Deer deer;
	bool showDeer;
public:
	App_Lighting(int w, int h);

	void program_step();


	void renderQuadric(const CGQuadric &quadric);
};


