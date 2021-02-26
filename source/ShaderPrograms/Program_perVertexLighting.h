#pragma once
#include "CGContext/CGProgram.h"
#include "CGContext/CGProgramInterface.h"
#include "CGContext/CGMath.h"
#include <algorithm>
class Program_perVertexLighting : public CGProgram
{
public:
	struct
	{
		// define a the reflection properties of a material
		// parameters contain the necessary parameters for classical
		// blinn-phong reflection model
		struct MaterialData
		{
			CGVec4 ambient;
			CGVec4 diffuse;
			CGVec4 specular;
			CGVec4 emissive;
			float shininess;

			MaterialData()
			{
				ambient  = CGVec4(0.1f, 0.1f, 0.1f, 1.0f);
				diffuse  = CGVec4(0.7f, 0.7f, 0.7f, 1.0f);
				specular = CGVec4(0.3f, 0.3f, 0.3f, 1.0f);
				shininess = 8.0f;
			}
		};

		// define a the properties of a light source
		// parameters contain the necessary parameters for classical
		// blinn-phong reflection model
		struct LightData
		{
			CGVec4 ambient;
			CGVec4 diffuse;
			CGVec4 specular;
			CGVec4 position_es;
			CGVec4 spot_direction;
			float spot_cutoff;
			float spot_exponent;

			LightData()
			{
				spot_direction = CGVec4(0.0f,0.0f,-1.0f,0.0f);
				spot_exponent = 0.0f;
				spot_cutoff = 180.0f;
				position_es = CGVec4(0.0f,0.0f,0.0f,1.0f);
			}
		};

		// the actual uniforms
		MaterialData material;
		LightData light;
		CGMatrix4x4 projectionMatrix;
		CGMatrix4x4 modelViewMatrix;
		CGMatrix4x4 normalMatrix;
		CGTexture2D* texture;

		// helper to calculate the normalMatrix from the modelViewMatrix
		// call this whenever after you set a new modelViewMatrix!
		void updateNormalMatrix()
		{
			normalMatrix = modelViewMatrix;
			for(unsigned int i = 0; i< 3;i++){
				normalMatrix.at(3,i) = 0.0f;
				normalMatrix.at(i,3) = 0.0f;
			}
			normalMatrix.at(3,3)=1.0f;

			normalMatrix.invert();
			normalMatrix.transpose();
		}

	}
	uniform;
	Program_perVertexLighting()
	{
		uniform.texture = nullptr;
	}
	~Program_perVertexLighting(){}

	void vertex_shader(const CGVertexAttributes& in,
	                   CGVaryings& out) const
	{

		CGVec4 ambi(0.0f), diff(0.0f), spec(0.0f);

		//ambi = CGVec4(0.5f); // TODO: change this!
		ambi = uniform.light.ambient * uniform.material.ambient;

		// Transform from Object Space into Eye Space.
		CGVec4 vPos = uniform.modelViewMatrix * in.position;
		// U10 A2 & A3
		
		CGVec4 vNrm = uniform.normalMatrix * in.normal;
		vNrm = CGMath::normalize(vNrm);
		// L is vector direction from current point (vPos) to the light source (uniforms.lightPosition[0])
		CGVec4 L = CGMath::normalize(uniform.light.position_es - vPos);
		// calculate dot product of nrm and L
		float NdotL = CGMath::dot(vNrm, L);

		// diffuse
		if (NdotL > 0.0f) {

			diff = uniform.light.diffuse * uniform.material.diffuse * NdotL;
			// E is direction from current point (vPos) to eye position
			CGVec4 E = CGMath::normalize(CGVec4(0.0f, 0.0f, 0.0f, 1.0f) - vPos);
			// H is halfway vector between L and E
			CGVec4 H = CGMath::normalize(L + E);
			// specular
			float NdotH = CGMath::dot(vNrm, H);
			NdotH = std::max(NdotH, 0.0f);
			spec = uniform.light.specular * uniform.material.specular * std::pow(NdotH, uniform.material.shininess);
		}
		// sum up the final output color
		out.color = ambi + diff + spec;
		// Explicitly set alpha of the color
		out.color.w = uniform.material.diffuse.w;
		// clamp color values to range [0,1]
		//out.color = ...
		out.color = CGMath::clamp(out.color, CGVec4(0.0f, 0.0f, 0.0f, 0.0f), CGVec4(1.0f, 1.0f, 1.0f, 1.0f));
		// Transform from Eye Space into Clip Space.
		out.position = uniform.projectionMatrix * vPos;

		// forward tex-coords as varying
		// ...
		out.texcoord = in.texcoord;

	}

	void fragment_shader(const CGVaryings& in,
	                     CGVec4& out) const
	{
		//out = in.color;
		if (nullptr != uniform.texture) {
			out = uniform.texture->sample(in.texcoord);
		}else out = in.color;
	}
};

