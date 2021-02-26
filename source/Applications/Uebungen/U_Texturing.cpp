#include "U_Texturing.h"
#include "CGContext/CGMath.h"
#include "Utilities/CG1Helper/CG1Helper.h"
#include "Utilities/CGImageFile.h"

//------------------------------------------------------------------------------
App_Texturing::App_Texturing(int w, int h):Application (w,h)
{
	context.clearColor = CGVec4(0.0f, 0.0f, 0.0f, 1.0f);
	context.depthTestIsEnabled= true;
	context.bresenhamIsEnabled = true;
	context.backFaceCullingIsEnabled = true;
	context.polygonMode = CG_FILL;
	context.useProgram(prog_lit_tex);
	// set LIGHT properties (uniforms) with
	prog_lit_tex.uniform.light.ambient = CGVec4(0.1f);
	prog_lit_tex.uniform.light.diffuse = CGVec4(0.8f);
	prog_lit_tex.uniform.light.specular = CGVec4(0.8f);

	prog_lit_tex.uniform.material.ambient = CGVec4(1.0f);
	prog_lit_tex.uniform.material.diffuse = CGVec4(1.0f);
	prog_lit_tex.uniform.material.specular = CGVec4(1.0f);
	prog_lit_tex.uniform.material.shininess = 64.0f;


	// set projection matrix
	prog_lit_tex.uniform.projectionMatrix = CGMatrix4x4::getFrustum(-1.5f, 1.5f, -1.0f, 1.0f, 1.0f, 50.0f);
	prog_unlit.uniform.projectionMatrix = prog_lit_tex.uniform.projectionMatrix;

	sphere.setStandardColor(CGVec4(1.0f));
	sphere.createIcoSphere(2);


	// load the textures
	int img_w,img_h;
	unsigned char *imgData4ub = cgImageLoad("textures/checker.tga", &img_w, &img_h);
	test_texture.allocate(img_w,img_h,imgData4ub);
	free(imgData4ub); // the data was copied, we don't need to keep it

	imgData4ub = cgImageLoad("textures/deer.tga", &img_w, &img_h);
	deer_texture.allocate(img_w,img_h,imgData4ub);
	free(imgData4ub); // the data was copied, we don't need to keep it

	// set the texture parameters
	// ...
	test_texture.filterMode = CG_NEAREST;
	test_texture.wrapMode = CG_REPEAT; // TODO: try different wrap mode

	deer_texture.filterMode = CG_NEAREST;
	deer_texture.wrapMode = CG_CLAMP;

	showDeer=true;
}
//------------------------------------------------------------------------------
void App_Texturing::program_step()
{
	if (CG1Helper::isKeyReleased('w'))
		context.polygonMode = (context.polygonMode == CG_FILL?CG_LINE:CG_FILL);
	if (CG1Helper::isKeyReleased('r'))
		showDeer=!showDeer;

	context.clear(CG_COLOR_BUFFER_BIT | CG_DEPTH_BUFFER_BIT);
	// set modelview matrix and normal matrix
	context.useProgram(prog_lit_tex);
	CGMatrix4x4 view = CGMatrix4x4::getLookAt(CGVec4(0, 2, 2),CGVec4(0, 0, 0),CGVec4( 0, 1, 0));
	CGMatrix4x4 model = CGMatrix4x4::getRotationMatrixY(40);


	CGMatrix4x4 modelview = view * model;

	prog_lit_tex.uniform.modelViewMatrix = modelview;
	prog_lit_tex.uniform.updateNormalMatrix();



	float anim = 0.5f * CG1Helper::getTime();
	CGVec4 lightPos(cosf(anim) * 1.4f, 1.4f, sinf(anim) * 1.4f, 1);

	prog_lit_tex.uniform.light.position_es = modelview * lightPos;

	context.vertexAttributePointer.position = verticesQuad;
	context.vertexAttributePointer.normal = normalsQuad;
	context.vertexAttributePointer.color = colorsQuad;
	context.vertexAttributePointer.texcoord = texcoordsQuad1;
	context.vertexAttributePointer.texcoord = texcoordsQuad2;

	prog_lit_tex.uniform.texture = &test_texture;
	context.drawArrays(CG_TRIANGLES, 0, 2 * 3);

	if (showDeer) {
		model = CGMatrix4x4::getRotationMatrixY(33*anim);
		modelview = view * model;

		prog_lit_tex.uniform.modelViewMatrix = modelview;
		prog_lit_tex.uniform.updateNormalMatrix();
		prog_lit_tex.uniform.texture = &deer_texture;
		deer.render(context);
	}

	/// Visualize the light source smaller sphere, use a shader for unlit colored rendering
	context.useProgram(prog_unlit);
	modelview  = view;
	modelview = modelview * CGMatrix4x4::getTranslationMatrix(lightPos[0], lightPos[1], lightPos[2]);
	modelview = modelview * CGMatrix4x4::getScaleMatrix(0.05f, 0.05f, 0.05f);
	prog_unlit.uniform.modelViewMatrix = modelview;
	renderQuadric(sphere);
}
//------------------------------------------------------------------------------
void App_Texturing::renderQuadric(const CGQuadric &quadric)
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
