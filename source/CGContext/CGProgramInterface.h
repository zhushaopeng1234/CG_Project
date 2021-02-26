#pragma once
#include "CGMatrix.h"
#include "CGVector.h"
#include "CGTexture.h"
#include "CG.h"

//---------------------------------------------------------------------------
/// Vertex attribute container (vertex processing inputs).
struct CGVertexAttributes
{
	CGVec4 position;
	CGVec4 normal;
	CGVec4 color;
	CGVec4 texcoord;
};

/// Vertex varying container (vertex processing outputs).
struct CGVaryings
{
	CGVec4 position;
	CGVec4 normal;
	CGVec4 color;
	CGVec4 texcoord;
	CGVec4 position_es;
};

/// Fragment data container.
struct CGFragmentData
{
	CGVec2i coordinates;   /// Fragment specific framebuffer coordinate
	CGVaryings varyings;   /// Varyings as copied from vertex or interpolated.

	void set(const CGVaryings& A)
	{
		varyings = A;
	}

	void set(const CGVaryings& A, const CGVaryings& B, float ratio)
	{
		/*
		const float ir = 1.0f - ratio;
		const float r = ratio;
		varyings.position = ir * A.position + r * B.position;
		varyings.color = ir * A.color + r * B.color;
		varyings.normal = ir * A.normal + r * B.normal;
		varyings.texcoord = ir * A.texcoord + r * B.texcoord;
		varyings.position_es = ir * A.position_es + r * B.position_es;
		*/
		float alpha = (1.0f - ratio) / A.position.w;
		float beta = ratio / B.position.w;

		varyings.position = (1 - ratio) * A.position + ratio * B.position;

		varyings.color = (alpha * A.color + beta * B.color) / (alpha + beta);
		varyings.normal = (alpha * A.normal + beta * B.normal) / (alpha + beta);
		varyings.texcoord = (alpha * A.texcoord + beta * B.texcoord) / (alpha + beta);
		varyings.position_es = (alpha * A.position_es + beta * B.position_es) / (alpha + beta);
	}

	void set(const CGVaryings& A, const CGVaryings& B, const CGVaryings& C, float alpha, float beta, float gamma)
	{
		// Übung 05, Aufgabe 2a).
		//Uebung 11 3)a , b
		// Hinweis: Beachten Sie, dass alle Attribute des Varyings interpoliert werden müssen:
		// - position
		// - color
		// - normal
		// - texcoord
		// - position_es
		varyings.position = alpha * A.position + beta * B.position + gamma * C.position;

		alpha *= A.position.w;
		beta *= B.position.w;
		gamma *= C.position.w;
		float sum = alpha + beta + gamma;

		varyings.color = (alpha * A.color + beta * B.color + gamma * C.color) / sum;
		varyings.normal = (alpha * A.normal + beta * B.normal + gamma * C.normal) / sum;
		varyings.texcoord = (alpha * A.texcoord + beta * B.texcoord + gamma * C.texcoord) / sum;
		varyings.position_es = (alpha * A.position_es + beta * B.position_es + gamma * C.position_es) / sum;
	}
};

