#include "U_JumpingLine.h"
#include "Utilities/CG1Helper/CG1Helper.h"
//------------------------------------------------------------------------------
void App_JumpingLine::animateJumpingPoint(
                App_JumpingLine::JumpingPoint &p)
{
	if (p.x >= frame_width -1) p.dx = -fabsf(p.dx);
	if (p.x <=           0.0f) p.dx =  fabsf(p.dx);
	if (p.y >= frame_height-1) p.dy = -fabsf(p.dy);
	if (p.y <=           0.0f) p.dy =  fabsf(p.dy);
	p.x+=p.dx;
	p.y+=p.dy;
}
//------------------------------------------------------------------------------
App_JumpingLine::App_JumpingLine(int w, int h)
	:Application (w,h)
{
	frame_width = w;
	frame_height = h;

	// prepare the data for the simulation
	a={1.0f, 1.0f, 20.0f, 0.0f};
	b={0.5f, -0.75f, 70.0f, 50.0f};

	// prepare vertex array for point a
	color[0][0]=1.0f;
	color[0][1]=1.0f;
	color[0][2]=1.0f;
	color[0][3]=1.0f;

	position[0][2]=0.0f;

	// prepare vertex array for point b
	color[1][0]=0.0f;
	color[1][1]=1.0f;
	color[1][2]=0.0f;
	color[1][3]=1.0f;

	position[1][2]=0.0f;

	// set the background color

	context.clearColor = CGVec4(0.2f, 0.2f, 0.2f, 1.0f);
}
//------------------------------------------------------------------------------
void App_JumpingLine::program_step()
{
	if (CG1Helper::isKeyReleased(GLFW_KEY_LEFT))  a.dx = -1.0f;
	if (CG1Helper::isKeyReleased(GLFW_KEY_RIGHT)) a.dx =  1.0f;
	if (CG1Helper::isKeyReleased(GLFW_KEY_UP))    a.dy =  1.0f;
	if (CG1Helper::isKeyReleased(GLFW_KEY_DOWN))  a.dy = -1.0f;
	animateJumpingPoint(a);
	animateJumpingPoint(b);

	// render

	// clear the screen
	context.clear(CG_COLOR_BUFFER_BIT);

	// update vertex position for point a

	position[0][0]=a.x;
	position[0][1]=a.y;

	// update vertex position for point b
	position[1][0]=b.x;
	position[1][1]=b.y;

	// set the vertex attribute pointers
	context.vertexAttributePointer.position = &position[0][0];
	context.vertexAttributePointer.color =&color[0][0];

	// select the program used
	context.useProgram(prog);

	// the draw call
	context.drawArrays(CG_LINES, 0, 2);
}




