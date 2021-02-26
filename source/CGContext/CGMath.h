#pragma once
#include  "CG.h"
#include "CGVector.h"
#include "CGMatrix.h"
#include <cmath>
#include <cstdlib>
#ifndef M_PI
#define M_PI 3.1415926535897932384
#endif
#include <stdio.h>

#define ISZERO(x) (fabs(x)<1.0e-5)

namespace  CGMath
{
inline float dot(const CGVec4& a, const CGVec4& b)
{
	return a[0]*b[0]+a[1]*b[1]+a[2]*b[2]+a[3]*b[3];
}
inline CGVec4 cross(const CGVec4& a, const CGVec4& b)
{
	CGVec4 c;
	c[0]=a[1]*b[2]-a[2]*b[1];
	c[1]=a[2]*b[0]-a[0]*b[2];
	c[2]=a[0]*b[1]-a[1]*b[0];
	c[3]=0.0f;
	return c;
}
inline float length(const CGVec4& a)
{
	return sqrtf(dot(a,a));
}
inline CGVec4 normalize(const CGVec4& a)
{
	return a / length(a);
}
inline CGVec4 divw(const CGVec4& a)
{
	CGVec4 c;
	float s=1.0f/a[3];
	c[0]=s*a[0]; c[1]=s*a[1]; c[2]=s*a[2]; c[3]=1.0f;
	return c;
}
inline CGVec4 max(const CGVec4& a, float m)
{
	CGVec4 c;
	for (unsigned int i=0; i<4; i++)
		c[i]=a[i]>m?a[i]:m;
	return c;
}
inline CGVec4 min(const CGVec4& a, float n)
{
	CGVec4 c;
	for (unsigned int i=0; i<4;i++)
		c[i]=a[i]>n?n:a[i];
	return c;
}
inline CGVec4 max(const CGVec4& a, const CGVec4& b)
{
	CGVec4 c;
	for (unsigned int i=0; i<4; i++)
		c[i]=a[i]>b[i]?a[i]:b[i];
	return c;
}
inline CGVec4 min(const CGVec4& a, const CGVec4& b)
{
	CGVec4 c;
	for (unsigned int i=0; i<4; i++)
		c[i]=a[i]>b[i]?b[i]:a[i];
	return c;
}
inline CGVec4 clamp(const CGVec4& a, float m, float n)
{
	return max(min(a,n),m);
}
inline CGVec4 clamp(const CGVec4& a, const CGVec4& m, const CGVec4& n)
{
	return max(min(a,n),m);
}
inline CGVec4 abs(const CGVec4& a)
{
	CGVec4 c;
	for (unsigned int i=0; i<4; i++)
		c[i]=fabsf(a[i]);
	return c;
}

inline CGVec4 ceil(const CGVec4& a)
{
	CGVec4 c;
	for (unsigned int i=0; i<4; i++)
		c[i]=std::ceil(a[i]);
	return c;
}
inline CGVec4 floor(const CGVec4& a)
{
	CGVec4 c;
	for (unsigned int i=0; i<4; i++)
		c[i]=std::floor(a[i]);
	return c;
}
inline CGVec4 fract(const CGVec4& a)
{
	CGVec4 c;
	for (unsigned int i=0; i<4; i++)
		c[i]=a[i]-std::floor(a[i]);
	return c;
}
inline float distance(const CGVec4& a, const CGVec4& b)
{
	return length(a-b);
}

inline CGVec2 abs(const CGVec2& a)
{
	CGVec2 c;
	for (unsigned int i=0; i<2; i++)
		c[i]=fabsf(a[i]);
	return c;
}

inline CGVec2i abs(const CGVec2i& a)
{
	CGVec2i c;
	for (unsigned int i=0; i<2; i++)
		c[i]=::abs(a[i]);
	return c;
}



inline float length(const CGVec2& a)
{
	return sqrtf(a.x*a.x + a.y*a.y);
}


inline CGVec2i max(const CGVec2i& a, const CGVec2i& b)
{
	return CGVec2i(a.x > b.x?a.x: b.x,
	               a.y > b.y?a.y: b.y);

}
inline CGVec2i min(const CGVec2i& a, const CGVec2i& b)
{
	return CGVec2i(a.x < b.x?a.x: b.x,
	               a.y < b.y?a.y: b.y);
}

inline CGVec2i max(const CGVec2i& a, const int& b)
{
	return CGVec2i(a.x > b?a.x: b,
	               a.y > b?a.y: b);

}
inline CGVec2i min(const CGVec2i& a, const int& b)
{
	return CGVec2i(a.x < b?a.x: b,
	               a.y < b?a.y: b);
}

inline CGVec2i clamp(const CGVec2i& a, int m, int n)
{
	return max(min(a,n),m);
}

inline CGVec2i clamp(const CGVec2i& a, const CGVec2i& m, const CGVec2i& n)
{
	return max(min(a,n),m);
}



};
