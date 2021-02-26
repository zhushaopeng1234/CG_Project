#include "U_TestViewport.h"
//------------------------------------------------------------------------------
App_TestViewport::App_TestViewport(int w, int h)
	:Application(w,h)
{
	frame_width = w;
	frame_height = h;
	context.clearColor = CGVec4(0.0f, 0.0f, 0.0f, 1.0f);
	context.bresenhamIsEnabled = true;
}
//------------------------------------------------------------------------------
void App_TestViewport::program_step()
{
	// clear the screen
	context.clear(CG_COLOR_BUFFER_BIT);

	// set the vertex attribute pointers
	context.vertexAttributePointer.position = vertexPosition_TestTriangle;
	context.vertexAttributePointer.color = vertexColor_TestTriangle;

	context.useProgram(prog);

	// U07 A2b)
	// Hinweise:
	//   - aktuell wird ein viewport auf die Größe des Framebuffers gesetzt.
	//   - Ändern Sie die Parameter in der set Funktion um die Größe des Viewports zu ändern
	//   - Achten Sie darauf, dass sie die Breite und Höhe angeben müssen, nicht die rechte obere Ecke:
	//   - context.viewport.set(x_links_unten, y_links_unten, Breite, Höhe)
	//   - Rufen Sie die beiden folgenden Befehle mehrfach auf		

	//context.viewport.set(0, 0, frame_width, frame_height);
	//context.drawArrays(CG_TRIANGLES, 0, 6); // 6 vertices for 2 triangle.
	const int viewportWidth = frame_width / 2;
	const int viewportHeight = frame_height / 2;

	//two methods
	context.viewport.set(0,0,frame_width / 2,frame_height / 2 );//CGRectangle(0,0,viewportWidth,viewportHeight);
	context.drawArrays(CG_TRIANGLES, 0, 6); // 6 vertices for 2 triangle.
	context.viewport.set(0, viewportHeight, viewportWidth, viewportHeight);
	context.drawArrays(CG_TRIANGLES, 0, 6); // 6 vertices for 2 triangle.
	context.viewport.set(viewportWidth, 0, viewportWidth, viewportHeight);
	context.drawArrays(CG_TRIANGLES, 0, 6); // 6 vertices for 2 triangle.
	context.viewport.set(viewportWidth, viewportHeight, viewportWidth, viewportHeight);
	context.drawArrays(CG_TRIANGLES, 0, 6); // 6 vertices for 2 triangle.

}
