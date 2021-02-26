#pragma once

#include <cassert>
#include <cstdio>
#include "CG.h"
/// Simple vector container, four elements.
struct CGVec4
{
	float x,y,z,w;
	const float & operator[] (const unsigned int i) const
	{
		assert(i<4);
		return (&x)[i];
	}
	float & operator[] (const unsigned int i)
	{
		assert(i<4);
		return (&x)[i];
	}
	void set(const float a, const float b, const float c, const float d)
	{
		x=a;
		y=b;
		z=c;
		w=d;
	}
	void set(const CGVec4& vec)
	{
		for(unsigned int i = 0; i< 4;i++)
			(&x)[i] = vec[i];
	}
	void print(const char* prefix = nullptr) const
	{
		if(prefix == nullptr)
			prefix = "";
		printf("%s: (%f)\n",prefix,(double) x);
		printf("%s: (%f)\n",prefix,(double) y);
		printf("%s: (%f)\n",prefix,(double) z);
		printf("%s: (%f)\n",prefix,(double) w);
	}


	/// special constructors
	CGVec4()
	{
		// empty constructor, leave elements uninitialized
		// use this if you are going to set/overwrite all elements anyways
	}

	CGVec4(const float v)
	{
		set(v,v,v,v);
	}

	CGVec4(const float a, const float b, const float c, const float d = 1.0f)
	{
		set(a,b,c,d);
	}

	CGVec4(const CGVec4& vec)
	{
		set(vec);
	}

	/// mathematic operators
	CGVec4 operator+(const CGVec4 &v) const
	{
		return CGVec4(x+v.x, y+v.y,z+v.z,w+v.w);
	}
	CGVec4 operator-(const CGVec4 &v) const
	{
		return CGVec4(x-v.x, y-v.y,z-v.z,w-v.w);
	}
	CGVec4 operator*(const CGVec4 &v) const
	{
		return CGVec4(x*v.x, y*v.y,z*v.z,w*v.w);
	}
	CGVec4 operator*(const float s) const
	{
		return CGVec4(x*s,y*s,z*s,w*s);
	}
	CGVec4 operator/(const float s) const
	{
		return *this * (1.0f/s);
	}

	CGVec4& operator+=(const CGVec4 &v)
	{
		for(unsigned int i = 0; i< 4;i++)
			(*this)[i] += v[i];
		return  *this;
	}
	CGVec4& operator-=(const CGVec4 &v)
	{
		for(unsigned int i = 0; i< 4;i++)
			(*this)[i] -= v[i];
		return  *this;
	}
	CGVec4& operator*=(const CGVec4 &v)
	{
		for(unsigned int i = 0; i< 4;i++)
			(*this)[i] *= v[i];
		return  *this;
	}
	CGVec4& operator*=(const float s)
	{
		for(unsigned int i = 0; i< 4;i++)
			(*this)[i] *= s;
		return  *this;
	}
	CGVec4& operator/=(const float s)
	{
		for(unsigned int i = 0; i< 4;i++)
			(*this)[i] /=s;
		return  *this;
	}


	float* getDataPtr(){return &x;}
	const float* getDataPtr() const {return &x;}
};

inline CGVec4 operator*(float s, const CGVec4 &v)
{
	return v * s;
}
//
/// Simple (float) vector with two elements.
struct CGVec2
{
	float x,y;
	float& operator[] (const unsigned int i) { return (&x)[i]; }
	const float& operator[] (const unsigned int i) const { return (&x)[i]; }
	void set(const float a, const float b)
	{
		x=a;
		y=b;
	}
	void set(const CGVec2& vec)
	{
		x=vec.x;
		y=vec.y;
	}
	void set(const float a)
	{
		set(a,a);
	}
	/// special constructors
	CGVec2()
	{
		// empty constructor, leave elements uninitialized
		// use this if you are going to set/overwrite all elements anyways
	}
	CGVec2(const float a, const float b)
	{
		set(a,b);
	}
	CGVec2(const CGVec2& vec)
	{
		set(vec);
	}
	/// mathematic operators
	CGVec2 operator+(const CGVec2 &v) const
	{
		return CGVec2(x+v.x, y+v.y);
	}
	CGVec2 operator-(const CGVec2 &v) const
	{
		return CGVec2(x-v.x, y-v.y);
	}
	CGVec2 operator*(const CGVec2 &v) const
	{
		return CGVec2(x*v.x, y*v.y);
	}
	CGVec2 operator*(const float s) const
	{
		return CGVec2(x*s,y*s);
	}
	CGVec2 operator/(const float s) const
	{
		return *this * (1.0f/s);
	}

	CGVec2& operator+=(const CGVec2 &v)
	{
		for(unsigned int i = 0; i< 2;i++)
			(*this)[i] += v[i];
		return  *this;
	}
	CGVec2& operator-=(const CGVec2 &v)
	{
		for(unsigned int i = 0; i< 2;i++)
			(*this)[i] -= v[i];
		return  *this;
	}
	CGVec2& operator*=(const CGVec2 &v)
	{
		for(unsigned int i = 0; i< 2;i++)
			(*this)[i] *= v[i];
		return  *this;
	}
	CGVec2& operator*=(const float s)
	{
		for(unsigned int i = 0; i< 2;i++)
			(*this)[i] *= s;
		return  *this;
	}
	CGVec2& operator/=(const float s)
	{
		for(unsigned int i = 0; i< 2;i++)
			(*this)[i] /=s;
		return  *this;
	}


};

/// Simple (integer) vector with two elements.
struct CGVec2i
{
	int x,y;
	int& operator[] (const unsigned int i)
	{
		assert(i<2);
		return (&x)[i];
	}
	const int & operator[] (const unsigned int i) const
	{
		assert(i<2);
		return (&x)[i];
	}
	void set(const int a, const int b)
	{
		x=a;
		y=b;
	}
	void set(const CGVec2i& vec)
	{
		x=vec.x;
		y=vec.y;
	}
	void print() const
	{
		printf("(%d,%d)",x,y);
	}
	/// special constructors
	CGVec2i()
	{
		// empty constructor, leave elements uninitialized
		// use this if you are going to set/overwrite all elements
		// anyways
	}
	CGVec2i(const int v)
	{
		set(v,v);
	}
	CGVec2i(const int a, const int b)
	{
		set(a,b);
	}
	CGVec2i(const CGVec2i& vec)
	{
		set(vec);
	}

	CGVec2i(const CGVec4& vec)
	{
		set((int)vec.x,(int)vec.y);
	}

	bool operator==(const CGVec2i& o) const
	{
		return x == o.x && y == o.y;
	}
	CGVec2i operator+(const CGVec2i& o) const
	{
		return CGVec2i(x+o.x,y+o.y);
	}

	CGVec2i operator-(const CGVec2i& o) const
	{
		return CGVec2i(x-o.x,y-o.y);
	}

	CGVec2i operator*(const CGVec2i& o) const
	{
		return CGVec2i(x*o.x,y*o.y);
	}


	CGVec2i operator*(const int o) const
	{
		return CGVec2i(x*o,y*o);
	}
	CGVec2i operator*(const unsigned int o) const
	{
		return CGVec2i(x*static_cast<int>(o),
		               y*static_cast<int>(o));
	}
};

