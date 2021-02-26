#include "U_TestRotatingTriangle.h"
#include "Utilities/CG1Helper/CG1Helper.h"
//------------------------------------------------------------------------------
App_TestRotatingTriangle::App_TestRotatingTriangle(int w, int h)
        :Application(w,h)
{
	context.clearColor = CGVec4(0.0f, 0.0f, 0.0f, 1.0f);
	context.bresenhamIsEnabled = true;
	
	// TODO: Uebung 05, Aufgabe 1a)
	//context.polygonMode = CG_LINE;
	context.polygonMode = CG_FILL;
}
//------------------------------------------------------------------------------
void App_TestRotatingTriangle::program_step()
{
	// clear the screen
	context.clear(CG_COLOR_BUFFER_BIT);

	// Construct vertex data
	const float step = 0.3f * CG1Helper::getTime();
	const float ca = cosf(step);
	const float sa = sinf(step);
	float vertexPosition_TestRotTriangle[3 * 9] =
	{
		20, 20, 0,
		ca * 20 + 20, sa * 20 + 20, 0,
		-sa * 15 + 20, ca * 15 + 20, 0,
		1, 1, 0,
		4, 1, 0,
		1, 4, 0,
		6.6f, 1.6f, 0,
		7.4f, 1.6f, 0,
		6.6f, 2.4f, 0
	};

	// set the vertex attribute pointers
	context.vertexAttributePointer.position = vertexPosition_TestRotTriangle;
	context.vertexAttributePointer.color = vertexColor_TestRotTriangle;

	// select the program used
	context.useProgram(prog);

	context.drawArrays(CG_TRIANGLES, 0, 9);
}
