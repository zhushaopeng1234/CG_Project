#include "CGTexture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CGMath.h"

//------------------------------------------------------------------------------
// CGTEXTURE2D
//------------------------------------------------------------------------------
CGTexture2D::CGTexture2D()
{
	reset();
}
//------------------------------------------------------------------------------
CGTexture2D::~CGTexture2D()
{
	free(m_data);
	m_data = nullptr;
}
//------------------------------------------------------------------------------
void CGTexture2D::reset()
{
	m_width=m_height=0;
	filterMode = CG_NEAREST;
	wrapMode = CG_CLAMP;
	m_data=nullptr;
}
//------------------------------------------------------------------------------
void CGTexture2D::allocate(int width, int height, const unsigned char* data4ub)
{
	this->m_width = width;
	this->m_height = height;
	m_data = (unsigned char*) realloc(m_data, (size_t)(width*height)*4);
	memcpy(m_data, data4ub, (size_t)(width*height)*4);
}

//------------------------------------------------------------------------------
CGVec4 CGTexture2D::fetchTexel(int x, int y) const
{
	// U11 A2a)
	// ...

	//return CGVec4(0.0f);
	
	CGVec4 sample;
	
	if (wrapMode == CG_CLAMP) {
		if (x < 0) x = 0;
		if (y < 0) y = 0;
		if (x > m_width - 1) x = m_width - 1;
		if (y > m_height - 1) y = m_height - 1;
	}
	else if (wrapMode == CG_REPEAT) {
		x %= m_width;
		y %= m_height;
	}

	sample[0] = (float)m_data[4 * y * m_width + 4 * x + 0] / 255.f;
	sample[1] = (float)m_data[4 * y * m_width + 4 * x + 1] / 255.f;
	sample[2] = (float)m_data[4 * y * m_width + 4 * x + 2] / 255.f;
	sample[3] = (float)m_data[4 * y * m_width + 4 * x + 3] / 255.f;

	return sample;
}
//------------------------------------------------------------------------------
CGVec4 CGTexture2D::sample(const CGVec4 &texcoord) const
{
	// U11 A2b)
	// ...
	//return CGVec4(0.0f);
	//PRJ 3
	CGVec4 sample;
	int xt = (int)(texcoord.x * m_width - 0.5f);
	int yt = (int)(texcoord.y * m_height - 0.5f);
	if (filterMode == CG_LINEAR)
	{
		CGVec4 t0 = fetchTexel(xt, yt);
		CGVec4 t1 = fetchTexel(xt + 1.0f, yt);
		CGVec4 t2 = fetchTexel(xt, yt + 1.0f);
		CGVec4 t3 = fetchTexel(xt + 1.0f, yt + 1.0f);
		float a = texcoord.x * m_width - 0.5f - xt;
		float b = texcoord.y * m_height - 0.5f - yt;
		sample = (1 - b) * ((1 - a) * t0 + a * t1) + b * ((1 - a) * t2 + a * t3);
	}
	else
	{
		sample = fetchTexel((int)(texcoord.x * m_width), (int)(texcoord.y * m_height));
	}
	return sample;
}
//------------------------------------------------------------------------------
