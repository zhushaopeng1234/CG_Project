#include "U_HappyHolidays.h"
#include "CGContext/CGMath.h"
#include "Utilities/CG1Helper/CG1Helper.h"
//------------------------------------------------------------------------------
App_HappyHolidays::App_HappyHolidays(int w, int h)
        : Application (w,h), deers(NUM_DEERS), snowflakes(NUM_SNOWFLAKES)
{
	context.clearColor = CGVec4(0.7f, 0.7f, 1.0f, 1.0f);

	// U09 A1 d)  enable the depth test
	// ...
	context.depthTestIsEnabled = true;
	context.backFaceCullingIsEnabled = true;
	context.bresenhamIsEnabled = true;
	context.blendingIsEnabled = true;
}
//------------------------------------------------------------------------------
void App_HappyHolidays::program_step()
{
	deers.updateAnimation(CG1Helper::getTimeDelta());
	snowflakes.updateAnimation(CG1Helper::getTimeDelta());

	context.useProgram(prog);

	if (CG1Helper::isKeyReleased('w'))
		context.polygonMode = (context.polygonMode == CG_FILL?CG_LINE:CG_FILL);

	if (CG1Helper::isKeyReleased('r'))
		context.depthTestIsEnabled = (context.depthTestIsEnabled == true ? false : true);


	context.clear(CG_COLOR_BUFFER_BIT | CG_DEPTH_BUFFER_BIT);

	prog.uniform.projectionMatrix = CGMatrix4x4::getFrustum(-0.062132f, 0.062132f, -0.041421f, 0.041421f, 0.1f, 50.0f);
	
	//CGMatrix4x4 view = CGMatrix4x4::getTranslationMatrix(0.0f, -5.0, -25.0f);
	///*
	// U08 A4a)
	// Bird's-eye view
	const float dt=CG1Helper::getTimeDelta();
	if (CG1Helper::isKeyPressed(GLFW_KEY_UP)) polar += 45.0f * dt;
	if (CG1Helper::isKeyPressed(GLFW_KEY_DOWN)) polar -= 45.0f * dt;
	if (CG1Helper::isKeyPressed(GLFW_KEY_LEFT)) azimuth += 45.0f * dt;
	if (CG1Helper::isKeyPressed(GLFW_KEY_RIGHT)) azimuth -= 45.0f * dt;

	// keep polar and azimuth within the limitations
	if (polar > 90.0f) polar = 90.0f;
	if (polar < 0.0f) polar = 0.0f;
	if (azimuth < 0.0f) azimuth += 360.0f;
	if (azimuth > 360.0f) azimuth -= 360.0f;

	//view =
	CGMatrix4x4 view = CGMatrix4x4::getTranslationMatrix(0.0f, -5.0f, -25.0f) * CGMatrix4x4::getRotationMatrixX(polar) * CGMatrix4x4::getRotationMatrixY(azimuth);
	//*/
	// U08 A4c)
	// Camera rotating around center on r = 15 circle.
	//float anim = CG1Helper::getTime() * 0.3f;
	//CGVec4 eye( cos(anim) * 15.0f, 15.0f, sin(anim) * 15.0f);
	//CGMatrix4x4 view = CGMatrix4x4::getLookAt(eye,CGVec4(0.0f, 2.0f, 0.0f), CGVec4(0.0f, 1.0f, 0.0f));

	drawGround(view);
	for (int i = 0; i < 10; i++) {
		float posX = float(i % 7) / 6.0f * 16.0f - 8.0f; // [0,6]->[-8,+8]
		float posZ = float(i % 4) / 3.0f * 16.0f - 8.0f; // [0,3]->[-8,+8]
		CGMatrix4x4 translationTree = CGMatrix4x4::getTranslationMatrix(posX, 0.0f, posZ);
		drawTree(view, translationTree);
	}
	snowflakes.render(context, prog.uniform.modelViewMatrix, view);
	renderDeers(view);
	// ...


	// U09 A2b) & c)
	CGMatrix4x4 scale = CGMatrix4x4::getScaleMatrix(5.0f, 5.0f, 5.0f);
	prog.uniform.modelViewMatrix = view * scale;
	drawColoredRect(0.2f, 0.65f, 0.5f, 0.5f);

	prog.uniform.modelViewMatrix = view * CGMatrix4x4::getTranslationMatrix(0.0f, 0.0f, -1.0f) * scale;
	//float alpha = (sinf(CG1Helper::getTime())+ 1.0f) / 2.0f;
	//drawColoredRect(0.8f, 0.35f, 0.9f, alpha);
	drawColoredRect(0.8f, 0.35f, 0.9f, 0.5f);

}
//------------------------------------------------------------------------------
void App_HappyHolidays::drawTree(const CGMatrix4x4 &view, const CGMatrix4x4 translation)
{
	/*
	CGMatrix4x4 modelViewTrunk, modelViewLeaf[3];
	CGMatrix4x4 rotationY;

	// U08 A3a)
	// ...
	// modelViewTrunk =
	// modelViewLeaf[0] =
	// modelViewLeaf[1] =
	// modelViewLeaf[2] =
	modelViewTrunk = view * translation * CGMatrix4x4::getRotationMatrixY(0.0f);
	modelViewLeaf[0] = view * translation * CGMatrix4x4::getTranslationMatrix(0.0f, 2.5f, 0.0f) * CGMatrix4x4::getRotationMatrixY(0.0f) * CGMatrix4x4::getRotationMatrixZ(-135.0f);
	modelViewLeaf[1] = view * translation * CGMatrix4x4::getTranslationMatrix(0.0f, 3.0f, 0.0f) * CGMatrix4x4::getRotationMatrixY(0.0f) * CGMatrix4x4::getRotationMatrixZ(-135.0f) * CGMatrix4x4::getScaleMatrix(0.8f, 0.8f, 0.8f);
	modelViewLeaf[2] = view * translation * CGMatrix4x4::getTranslationMatrix(0.0f, 3.5f, 0.0f) * CGMatrix4x4::getRotationMatrixY(0.0f) * CGMatrix4x4::getRotationMatrixZ(-135.0f) * CGMatrix4x4::getScaleMatrix(0.5f, 0.5f, 0.5f);
	
	// draw TRUNK
	prog.uniform.modelViewMatrix = modelViewTrunk;
	context.vertexAttributePointer.position = verticesTrunk;
	context.vertexAttributePointer.color = colorsTrunk;
	context.drawArrays(CG_TRIANGLES, 0, 3);

	// draw LEAVES
	context.vertexAttributePointer.position = verticesLeafs;
	context.vertexAttributePointer.color = colorsLeafs;
	for(int i = 0; i < 3; i++) {
		prog.uniform.modelViewMatrix = modelViewLeaf[i];
		context.drawArrays(CG_TRIANGLES, 0, 3);
	}
	*/
	
	for (int j = 0; j < 3; j++) {
		CGMatrix4x4 modelViewTrunk, modelViewLeaf[3];

		modelViewTrunk = view * translation * CGMatrix4x4::getRotationMatrixY(j * 60.0f);
		modelViewLeaf[0] = view * translation * CGMatrix4x4::getTranslationMatrix(0.0f, 2.5f, 0.0f) * CGMatrix4x4::getRotationMatrixY(j * 60.0f) * CGMatrix4x4::getRotationMatrixZ(-135.0f);
		modelViewLeaf[1] = view * translation * CGMatrix4x4::getTranslationMatrix(0.0f, 3.0f, 0.0f) * CGMatrix4x4::getRotationMatrixY(j * 60.0f) * CGMatrix4x4::getRotationMatrixZ(-135.0f) * CGMatrix4x4::getScaleMatrix(0.8f, 0.8f, 0.8f);
		modelViewLeaf[2] = view * translation * CGMatrix4x4::getTranslationMatrix(0.0f, 3.5f, 0.0f) * CGMatrix4x4::getRotationMatrixY(j * 60.0f) * CGMatrix4x4::getRotationMatrixZ(-135.0f) * CGMatrix4x4::getScaleMatrix(0.5f, 0.5f, 0.5f);
		
		// draw TRUNK
		prog.uniform.modelViewMatrix = modelViewTrunk;
		context.vertexAttributePointer.position = verticesTrunk;
		context.vertexAttributePointer.color = colorsTrunk;
		context.drawArrays(CG_TRIANGLES, 0, 3);

		// draw LEAVES
		context.vertexAttributePointer.position = verticesLeafs;
		context.vertexAttributePointer.color = colorsLeafs;
		for (int i = 0; i < 3; i++) {
			prog.uniform.modelViewMatrix = modelViewLeaf[i];
			context.drawArrays(CG_TRIANGLES, 0, 3);
		}
		
	}
	
}
//------------------------------------------------------------------------------
void App_HappyHolidays::drawGround(const CGMatrix4x4 &view)
{
	prog.uniform.modelViewMatrix = view;
	context.vertexAttributePointer.position = verticesGround;
	context.vertexAttributePointer.color = colorsGround;
	context.drawArrays(CG_TRIANGLES, 0, 6);
}
//------------------------------------------------------------------------------
void App_HappyHolidays::renderDeers(const CGMatrix4x4& view)
{
	for (size_t i = 0; i < deers.deer.size(); i++) {
		CGMatrix4x4 modelViewDeer = view * deers.deer[i].getModelMatrix();
		prog.uniform.modelViewMatrix = modelViewDeer;
		deers.deer[i].render(context);
	}
}
//------------------------------------------------------------------------------
void App_HappyHolidays::drawColoredRect(float r, float g, float b, float a)
{
	static const float vertexPositions[3 * 6] = {
	        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f
	};
	float colors[4 * 6];
	for (int i = 0; i < 6; i++) {
		colors[4 * i] = r; colors[4 * i + 1] = g; colors[4 * i + 2] = b; colors[4 * i + 3] = a;
	}
	context.vertexAttributePointer.position = vertexPositions;
	context.vertexAttributePointer.color = colors;
	context.drawArrays(CG_TRIANGLES, 0, 6);
	context.vertexAttributePointer.position = nullptr;
	context.vertexAttributePointer.color = nullptr;
}

