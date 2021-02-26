#include "U_AwesomeTriangle.h"
//------------------------------------------------------------------------------
App_AwesomeTriangle::App_AwesomeTriangle(int w, int h)
        :Application(w,h)
{

	context.clearColor = CGVec4(1.0f, 1.0f, 1.0f, 1.0f);
	context.bresenhamIsEnabled = true;
	context.blendingIsEnabled =true;


	const int X = 0, R = 0;
	const int Y = 1, G = 1;
	const int Z = 2, B = 2;
	const int        A = 3;

	vertices[3 * 0 + X] = -1.f; vertices[3 * 0 + Y] = -1.f; vertices[3 * 0 + Z] = 0.f;
	vertices[3 * 1 + X] = 1.f; vertices[3 * 1 + Y] = -1.f; vertices[3 * 1 + Z] = 0.f;
	vertices[3 * 2 + X] = 0.f; vertices[3 * 2 + Y] = 1.f; vertices[3 * 2 + Z] = 0.f;
	colors[4 * 0 + R] = 1.0f; colors[4 * 0 + G] = 0.0f; colors[4 * 0 + B] = 0.0f; colors[4 * 0 + A] = 0.2f;
	colors[4 * 1 + R] = 0.0f; colors[4 * 1 + G] = 1.0f; colors[4 * 1 + B] = 0.0f; colors[4 * 1 + A] = 0.2f;
	colors[4 * 2 + R] = 0.0f; colors[4 * 2 + G] = 0.0f; colors[4 * 2 + B] = 1.0f; colors[4 * 2 + A] = 0.2f;

}
//------------------------------------------------------------------------------
void App_AwesomeTriangle::program_step()
{
	const int X = 0;
	const int Y = 1;
	const int Z = 2;


	step += 0.01f;
	if (step >= 0.5)
		step = 0.0;

	float tempVertices[3 * 3];
	float tempColors[3 * 4];
	for (int i = 0; i < 19; i++) {
		memcpy(tempColors, &colors[3 * 4 * i], 3 * 4 * sizeof(float));
		tempVertices[3 * 0 + X] = step * vertices[3 * 3 * i + 3 * 1 + X] + (1 - step) * vertices[3 * 3 * i + 3 * 0 + X];
		tempVertices[3 * 0 + Y] = step * vertices[3 * 3 * i + 3 * 1 + Y] + (1 - step) * vertices[3 * 3 * i + 3 * 0 + Y];
		tempVertices[3 * 0 + Z] = step * vertices[3 * 3 * i + 3 * 1 + Z] + (1 - step) * vertices[3 * 3 * i + 3 * 0 + Z];
		tempVertices[3 * 1 + X] = step * vertices[3 * 3 * i + 3 * 2 + X] + (1 - step) * vertices[3 * 3 * i + 3 * 1 + X];
		tempVertices[3 * 1 + Y] = step * vertices[3 * 3 * i + 3 * 2 + Y] + (1 - step) * vertices[3 * 3 * i + 3 * 1 + Y];
		tempVertices[3 * 1 + Z] = step * vertices[3 * 3 * i + 3 * 2 + Z] + (1 - step) * vertices[3 * 3 * i + 3 * 1 + Z];
		tempVertices[3 * 2 + X] = step * vertices[3 * 3 * i + 3 * 0 + X] + (1 - step) * vertices[3 * 3 * i + 3 * 2 + X];
		tempVertices[3 * 2 + Y] = step * vertices[3 * 3 * i + 3 * 0 + Y] + (1 - step) * vertices[3 * 3 * i + 3 * 2 + Y];
		tempVertices[3 * 2 + Z] = step * vertices[3 * 3 * i + 3 * 0 + Z] + (1 - step) * vertices[3 * 3 * i + 3 * 2 + Z];
		memcpy(&vertices[3 * 3 * (i + 1)], tempVertices, 3 * 3 * sizeof(float));
		memcpy(&colors[3 * 4 * (i + 1)], tempColors, 3 * 4 * sizeof(float));
	}

	context.clear(CG_COLOR_BUFFER_BIT | CG_DEPTH_BUFFER_BIT);

	context.vertexAttributePointer.position = vertices;
	context.vertexAttributePointer.color = colors;
	context.useProgram(prog);
	context.drawArrays(CG_TRIANGLES, 0, 20 * 3);
}
