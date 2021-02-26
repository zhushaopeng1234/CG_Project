#include "U_Lighting.h"
#include "CGContext/CGMath.h"
#include "Utilities/CG1Helper/CG1Helper.h"
#include "Utilities/CGImageFile.h"
//------------------------------------------------------------------------------
App_Lighting::App_Lighting(int w, int h):Application (w,h)
{
	context.clearColor = CGVec4(0.0f, 0.0f, 0.0f, 1.0f);

	context.depthTestIsEnabled= true;
	context.backFaceCullingIsEnabled = true;
	context.bresenhamIsEnabled = true;
	context.polygonMode = CG_FILL;
	context.useProgram(prog_lit);

	// U10 A3b)
	// set LIGHT properties (uniforms)
	prog_lit.uniform.light.ambient = CGVec4(0.2f,0.2f,0.2f,1.0f);
	prog_lit.uniform.light.diffuse = CGVec4(1.0f);
	prog_lit.uniform.light.specular = CGVec4(1.0f);

	// set MATERIAL properties (uniforms)
	prog_lit.uniform.material.ambient = CGVec4(0.7f,0.3f,0.1f,1.0);
	prog_lit.uniform.material.diffuse = CGVec4(0.7f,0.3f,0.1f,1.0);
	prog_lit.uniform.material.specular = CGVec4(0.4f,0.2f,0.1f,1.0f);
	prog_lit.uniform.material.shininess = 16.0f;


	// set projection matrix
	prog_lit.uniform.projectionMatrix = CGMatrix4x4::getFrustum(-1.5f*0.7f, 1.5f*0.7f, -0.7f, 0.7f, 1.0f, 50.0f);
	prog_unlit.uniform.projectionMatrix =  prog_lit.uniform.projectionMatrix;


	sphere1.setStandardColor(CGVec4(0.0f, 1.0f, 0.0f, 1.0f));
	sphere1.createIcoSphere(4);
	sphere2.setStandardColor(CGVec4(1.0f));
	sphere2.createIcoSphere(2);

	showDeer = false;
}
//------------------------------------------------------------------------------
void App_Lighting::program_step()
{
	if (CG1Helper::isKeyReleased('w'))
		context.polygonMode = (context.polygonMode == CG_FILL?CG_LINE:CG_FILL);
	if (CG1Helper::isKeyReleased('r'))
		showDeer=!showDeer;

	context.clear(CG_COLOR_BUFFER_BIT | CG_DEPTH_BUFFER_BIT);
	// set modelview matrix and normal matrix
	context.useProgram(prog_lit);
	CGMatrix4x4 view = CGMatrix4x4::getLookAt(CGVec4(0, 2, 2),CGVec4(0, 0, 0),CGVec4( 0, 1, 0));
	CGMatrix4x4 model;
	if (showDeer) {
		model= CGMatrix4x4::getRotationMatrixY(40) * CGMatrix4x4::getTranslationMatrix(-0.5f, -0.4f, 0.5f);
	} else {
		model = CGMatrix4x4::getRotationMatrixY(40);
	}
	CGMatrix4x4 modelview = view * model;

	prog_lit.uniform.modelViewMatrix = modelview;
	prog_lit.uniform.updateNormalMatrix();

	float anim = 0.5f * CG1Helper::getTime();
	CGVec4 lightPos(cosf(anim) * 1.4f, 1.4f, sinf(anim) * 1.4f, 1);

	//prog_lit.uniform.light.position_es = modelview * lightPos;
	prog_lit.uniform.light.position_es = view * lightPos;

	if (showDeer) {
		deer.render(context);
	} else {
		renderQuadric(sphere1);
	}
	prog_lit.uniform.modelViewMatrix = view * CGMatrix4x4::getTranslationMatrix(-2.0f, 0.0f, 0.0f);

	/// Visualize the light source smaller sphere, use a shader for unlit colored rendering
	context.useProgram(prog_unlit);
	modelview  = view;
	modelview = modelview * CGMatrix4x4::getTranslationMatrix(lightPos[0], lightPos[1], lightPos[2]);
	modelview = modelview * CGMatrix4x4::getScaleMatrix(0.05f, 0.05f, 0.05f);
	prog_unlit.uniform.modelViewMatrix = modelview;
	renderQuadric(sphere2);
}
//------------------------------------------------------------------------------
void App_Lighting::renderQuadric(const CGQuadric &quadric)
{
	context.vertexAttributePointer.position = quadric.getPositionArray();
	context.vertexAttributePointer.normal = quadric.getNormalArray();
	context.vertexAttributePointer.color = quadric.getColorArray();
	context.vertexAttributePointer.texcoord = quadric.getTexCoordArray();
	context.drawArrays(CG_TRIANGLES, 0, quadric.getVertexCount());
	context.vertexAttributePointer.position = nullptr;
	context.vertexAttributePointer.normal = nullptr;
	context.vertexAttributePointer.color = nullptr;
	context.vertexAttributePointer.texcoord = nullptr;
}
