#include "U_TestBresenham.h"
//------------------------------------------------------------------------------
App_TestBresenham::App_TestBresenham(int w, int h)
	:Application(w,h)
{
	context.clearColor = CGVec4(0.0f, 0.0f, 0.0f, 1.0f);
	context.bresenhamIsEnabled = true;
}
//------------------------------------------------------------------------------
void App_TestBresenham::program_step()
{

	// clear the screen
	context.clear(CG_COLOR_BUFFER_BIT);

	// set the vertex attribute pointers
	context.vertexAttributePointer.position = vertexPosition_TestBresenham;
	context.vertexAttributePointer.color = vertexColor_TestBresenham;

	// select the program used
	context.useProgram(prog);
	context.drawArrays(CG_LINES, 0, 32);
}
