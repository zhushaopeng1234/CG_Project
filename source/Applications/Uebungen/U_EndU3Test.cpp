#include "U_EndU3Test.h"
#include <iostream>
#include "CGContext/CGMath.h"

App_EndU3Test::App_EndU3Test(int w, int h)
        :Application (w,h)
{
	error = false;
	context.clearColor = CGVec4(0.0f, 0.0f, 0.0f, 1.0f);
	// select the program used
	context.useProgram(prog);
	if(w != 83 || h != 41){
		std::cerr << "[!!!] The resolution should be set to 83x41!!\n \tPlease check the instructins and set the resolution accordingly!\n";
		error = true;
	}

}

void App_EndU3Test::program_step(){

	// clear the screen
	context.clear(CG_COLOR_BUFFER_BIT);

	// set the vertex attribute pointers
	context.vertexAttributePointer.position = vertexPosition_TestBresenham;
	context.vertexAttributePointer.color = vertexColor_TestBresenham;

	// render the first star using Bresenham
	context.bresenhamIsEnabled = true;
	context.drawArrays(CG_LINES, 0, 32);
	// render the second one using stupid line
	context.bresenhamIsEnabled = false;
	context.drawArrays(CG_LINES, 32, 32);

	unsigned char*d = context.m_frameBuffer.colorBuffer.getDataPointer();
	size_t size = (size_t)(context.m_frameBuffer.colorBuffer.getWidth()*context.m_frameBuffer.colorBuffer.getHeight()*4);

	for(size_t i = 0; i< size;i++){
		if(d[i] != fb_data[i] && !errors_handled)
		{
			std::cerr << "[!!!] At least one of your rasterizers has some issues! \n \tCheck CGRasterizer_line_stupid.cpp and CGRasterizer_line_bresenham.cpp and compare the output image with the image in the PDF of exercise three!"<<std::endl;
			error = true;
		}
	}


	// Test frame buffer get:
	CGVec4 color = context.m_frameBuffer.colorBuffer.get({0,20});
	if(CGMath::distance(color,CGVec4(1,0,0,1)) > 0.0001f && !errors_handled){
		std::cerr << "[!!!] Your CGFrameBuffer::CGColorBuffer::get() implementation is faulty! \n \tCheck CGFrameBuffer.cpp!\n"<<std::endl;
		error = true;
	}
	color = context.m_frameBuffer.colorBuffer.get({6,6});
	if(CGMath::distance(color,CGVec4(0,1,0,1)) > 0.0001f &&!errors_handled){
		std::cerr << "[!!!] Your CGFrameBuffer::CGColorBuffer::get() implementation is faulty! \n \tCheck CGFrameBuffer.cpp!\n"<<std::endl;
		error = true;
	}
	if(error){
		errors_handled = true;
	}
}




