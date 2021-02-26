#pragma once

#include "Applications/Application.h"

#include "CGContext/CGShape.h"
#include "ShaderPrograms/Program_perVertexLighting.h"
#include "ShaderPrograms/Program_modelViewProjection.h"

class App_Project : public Application
{
	struct SceneObject
	{
		CGShape* shape = nullptr;
		CGMatrix4x4 model_matrix;
		CGTexture2D* texture = nullptr;
		CGVec4 material_ambient = CGVec4(1.0f);
		CGVec4 material_diffuse = CGVec4(1.0f);
		CGVec4 material_specular = CGVec4(1.0f);
		float material_shininess = 64.0f;

		~SceneObject() { if (shape) delete shape; }
	};

	Program_perVertexLighting prog_lit_tex;
	Program_modelViewProjection prog_unlit;
	CGTexture2D texture_checker;
	CGMatrix4x4 view_matrix;

	CGSphereShape light_sphere;
	CGVec4 light_position;

	float camera_azimuth = 0.0f;
	float camera_inclination = 30.0f;
	float camera_radius = 20.0f;

	float perspective_horizontal = 1.6f;
	float perspective_vertical = 1.0f;
	float orthographic_horizontal = 8.0f;
	float orthographic_vertical = 5.0f;
	float projection_near = 1.0f;
	float projection_far = 50.0f;
	bool use_orthographic_projection = false;

	SceneObject* objects = nullptr;
	int num_objects = 0;

public:
	App_Project(int w, int h);
	virtual ~App_Project();

	virtual void program_step() override;

private:
	void load_texture();
	void initialize_scene_objects();
	void process_input();
	void calculate_view_matrix();
	void calculate_light_position();
	void draw_scene_objects();
	void draw_light_source();
	void set_projection_matrix();
};
