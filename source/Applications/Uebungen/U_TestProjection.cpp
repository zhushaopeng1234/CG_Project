#include "U_TestProjection.h"
#include "Utilities/CG1Helper/CG1Helper.h"
//------------------------------------------------------------------------------
void App_TestProjection::program_step()
{
	context.clear(CG_COLOR_BUFFER_BIT);

	if (CG1Helper::isKeyReleased('w')) wire=!wire;
	context.polygonMode = (wire?CG_LINE:CG_FILL);

	context.useProgram(prog);

	// calculate projection matrix and set it as a uniform in the
	// prog object
	// U07 A1b)
	// ...
	
	prog.uniform.projectionMatrix = CGMatrix4x4::getFrustum(-0.1f, 0.1f, -0.1f, 0.1f, 0.1f, 1.0f);;

	// draw the ground
	context.vertexAttributePointer.position = verticesGround;
	context.vertexAttributePointer.color = colorsGround;

	context.drawArrays(CG_TRIANGLES, 0, 2*3);

	// draw the cube
	context.vertexAttributePointer.position = verticesCube;
	context.vertexAttributePointer.color = colorsCube;

	context.drawArrays(CG_TRIANGLES, 0, 12*3);

}
//------------------------------------------------------------------------------
App_TestProjection::App_TestProjection(int w, int h)
	:Application(w,h)
{
	wire = false;
	context.bresenhamIsEnabled = true;
	context.clearColor = CGVec4(0.5f, 0.5f, 0.5f, 1.0f);

	// draw a ground plane:


	for(int i = 0; i < 6; i++) {
		float *c = colorsGround + 4 * i;
		c[0] = c[1] = c[2] = 0.8f; c[3]=1.0f;
	}

	// Create a unit-cube which is later (manually) transformed.
	// Hint: this is just a hack as long as we have no model transformation,
	// i. e. do not attempt this at home!
	for(int i = 0; i < 36; i++) {
		// compute colors from temporary vertex positions:
		int j = (i / 6) * 6;
		colorsCube[4 * i + 0] = verticesCubeBase[3 * j + 0];
		colorsCube[4 * i + 1] = verticesCubeBase[3 * j + 1];
		colorsCube[4 * i + 2] = verticesCubeBase[3 * j + 2];
		colorsCube[4 * i + 3] = 1.0f;
		// compute (manually!) transformed vertex positions:
		verticesCube[3 * i + 0] = verticesCubeBase[3 * i + 0] * 0.25f + 0.25f;
		verticesCube[3 * i + 1] = verticesCubeBase[3 * i + 1] * 0.25f - 0.5f;
		verticesCube[3 * i + 2] = verticesCubeBase[3 * i + 2] * 0.25f - 0.875f;
	}


}
