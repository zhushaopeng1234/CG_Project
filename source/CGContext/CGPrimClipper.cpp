#include "CGContext.h"
#include "CGPrimClipper.h"
#include <iostream>
#include <cassert>
#include <limits>

//---------------------------------------------------------------------------
// CLIPPING
//---------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Helper data structures and functions
//------------------------------------------------------------------------------
namespace	//anonymous
{
// These constants work as an interface to the CG.h
//	enum {_VERTEX_ATTR_CNT = CG_ATTRIBUTE_COUNT, _VERTEX_VARY_CNT = CG_VARYING_COUNT};

struct ClipBound
{
	enum {Left=0, Right, Bottom, Top, Near, Far, Cnt};
	// Pre-computation
	static const unsigned int BC[Cnt];        //boundary coordinate
	static const unsigned int Sign[Cnt];      //sign of boundary
};
const unsigned int ClipBound::BC[Cnt]   = {0,0,1,1,2,2}; //i/2
const unsigned int ClipBound::Sign[Cnt] = {0,1,0,1,0,1}; //i%2

bool isZero(float t)
{
	return t < 1E-5f && t > -1E-5f; //float epsilon
}

// Check if p is inside of the clipping boundary.
unsigned int isInside(unsigned int clipBound, const CGVec4 &p)
{
	float w = p[3];
	// Note:
	// Points with negative w are behind the camera (see the 4th row of projection matrix).
	// Division by a negative w produces incorrect x, y, z in R3, because
	// they will wrap around the negative infinitive and re-appear
	// in front of the camera.
	// To avoid this, points with a negative w are immediately considered outside of Near plane.
	// This rejection does not affect further processing of the clipping (such as intersection),
	// since a point behind the camera is also behind the actual Near plane, after all.
	if (clipBound == ClipBound::Near)
		return w>0 && p[2] >= -w;
	else
	{
		const unsigned int &bc = ClipBound::BC[clipBound];  // boundary coordinate
		return ClipBound::Sign[clipBound]?// 1: right, top, and far planes, 0: other
						  (w>0? p[bc] <=  w : p[bc] >=  w) :
						  (w>0? p[bc] >= -w : p[bc] <= -w);
	}
}

// Intersection between the extended line ab and the clipping boundary clipBound.
// Returns true if there is an intersection, false otherwise.
// If intersecting, the intersection is at a+t*(b-a).
// Note t is not necessarily in [0,1].
bool intersectLine(unsigned int clipBound, const CGVec4 *pa, const CGVec4 *pb, float &t)
{
	const CGVec4 &a = *pa;
	const CGVec4 &b = *pb;
	const unsigned int &bc   = ClipBound::BC[clipBound];
	const unsigned int &sign = ClipBound::Sign[clipBound];

	float aB, bB;        //distance from a to boundary, and from b to boundary
	if (sign) {
		aB = a[3]-a[bc];
		bB = b[3]-b[bc];
	}
	else {
		aB = -a[3]-a[bc];
		bB = -b[3]-b[bc];
	}

	float ab = aB - bB;  //signed distance from a to b in boundary coord.
	if (isZero(ab))
		return false;      //parallel to boundary

	// Compute ratio
	t = aB / ab;
	return true;
}

// Clip the line segment a+s*(b-a), s in [0,1] against the clipping boundary clipBound.
// Returns true if there is an intersection, false otherwise.
// fa and fb indicate if a and b are inside of the boundary.
// If intersecting, t is in [0,1] and the intersection is at a+t*(b-a).
// If not intersecting, t is not in [0,1] and
// fa and fb both indicate if the whole line lies in or outside of the boundary.
bool intersectSeg(unsigned int clipBound, const CGVec4 *pa, const CGVec4 *pb, float &t, unsigned int &fa, unsigned int &fb)
{
	const CGVec4 &a = *pa;
	const CGVec4 &b = *pb;
	fa = isInside(clipBound, a);
	fb = isInside(clipBound, b);

	if (!fa && !fb)
		return false;     //both outside

	const unsigned int &bc   = ClipBound::BC[clipBound];
	const unsigned int &sign = ClipBound::Sign[clipBound];

	float aB, bB;       //signed distance from a to boundary, and from b to boundary
	if (sign) {
		aB = a[3]-a[bc];
		bB = b[3]-b[bc];
	} else {
		aB = -a[3]-a[bc];
		bB = -b[3]-b[bc];
	}

	float ab = aB - bB; //signed distance from a to b in boundary coord.
	if (isZero(ab))
		return false;     //parallel to boundary

	// Compute ratio
	t = aB / ab;
	return t>=0.0f && t<=1.0f;
}

// Copy all attributes and varyings
void copyVert(const CGVaryings *vin, CGVaryings *vout)
{
	if (vin != vout && vin && vout)
	{
		*vout = *vin;
	}
}

// Attributes and varying will be linear interpolated in clip space, v = (1-t)*va+t*vb
// In-place operation possible (i.e, vout can be vin0 or vin1)
void interpolateVert(const CGVaryings *vin0, const CGVaryings *vin1, float t, CGVaryings *vout)
{
	if (vin0 && vin1 && vout)
	{
		const float t_ = 1.0f-t;
		vout->position = t_*vin0->position + t * vin1->position;
		vout->normal = t_*vin0->normal + t * vin1->normal;
		vout->color = t_*vin0->color + t * vin1->color;
		vout->texcoord = t_*vin0->texcoord + t * vin1->texcoord;
		vout->position_es = t_*vin0->position_es + t * vin1->position_es;

		//			for (unsigned int i=0; i<_VERTEX_VARY_CNT; ++i)
		//				for (unsigned int j=0; j<4; ++j)
		//					vout->varyings[i][j] =
		//						(1.0f-t)*vin0->varyings[i][j] + t*vin1->varyings[i][j];
	}
}
}//namespace anonymous

//----------------------------------------------------------------------------
// Init static members
//----------------------------------------------------------------------------
CGVaryings CGPrimClipper::sm_vout_buf[CG_MAX_VERTICES_IN_PIPELINE];
unsigned int CGPrimClipper::clip_bits = 0xffff;
//----------------------------------------------------------------------------
// Implementation of member functions
//----------------------------------------------------------------------------
const CGVaryings *CGPrimClipper::out_buf() {return &sm_vout_buf[0];}

unsigned int CGPrimClipper::clip_point(CGVaryings *vinBuf, int vinCnt)
{
	// In-place clipping(culling)
	CGVaryings *vin = vinBuf;
	CGVaryings *vout = vinBuf;
	unsigned int cnt = 0;

	for (int i=0; i<vinCnt; ++i, vin++) {
		bool reject = false;
		for (unsigned int clipBound=0; !reject && clipBound<6; ++clipBound)   //check against each boundaries
			if (!isInside(clipBound, vin->position))
				reject = true;

		if (reject)
			continue;

		// Not rejected
		copyVert(vin, vout++);
		cnt++;
	}

	return cnt;
}
//----------------------------------------------------------------------------
unsigned int CGPrimClipper::clip_line(CGVaryings *vinBuf, int vinCnt)
{
	// use Liang-Barsky line-clipping for now.
	return clip_line_LB(vinBuf, static_cast<int>(vinCnt));
}
//----------------------------------------------------------------------------
unsigned int CGPrimClipper::clip_poly(CGVaryings *vinBuf, int vinCnt)
{
	// use Sutherland-Hodgman polygon-clipping for now.
	return clip_poly_SH(vinBuf, static_cast<int>(vinCnt));
}
//----------------------------------------------------------------------------
unsigned int CGPrimClipper::clip_line_LB(CGVaryings *vinBuf, int vinCnt)
{
	CGVaryings *voutBuf = vinBuf;      //in-place clipping
	CGVaryings *vout = voutBuf;        //current output vertex
	unsigned int cnt = 0;                         //output vertex count
	CGVaryings *vs = nullptr, *ve = nullptr;       //the line segment to be checked is always vs->ve
	const CGVec4 *ps = nullptr, *pe = nullptr;       //positions of vertices, for boundary test and intersection
	CGVaryings vsclone;                //a copy of vs, for in-place modification of vs.
	unsigned int fs, fe;                          //inside-flags

	int lineCnt = vinCnt/2;              //extra vertices are ignored, if any
	for (int i=0; i<lineCnt; ++i)
	{
		bool reject = false;
		vs = vinBuf+2*i;
		ve = vs+1;
		ps = &vs->position;
		pe = &ve->position;

		// Let PE and PL be the parameter t of potentially entering and leaving lines,
		// where t in [0,1]. We can find the largest PE, t_pe, and smallest PL, t_pl.
		// To maximize the visible part of the line, the clipped line corresponds
		// to the parameter in the range [t_pe, t_pl].
		float t=0.0f, t_pe = 0.0f, t_pl = 1.0f;

		// Clip against each boundary.
		for (unsigned int clipBound=0; !reject && clipBound<6; ++clipBound) {
			bool inter = intersectSeg(clipBound, ps, pe, t, fs, fe);
			unsigned int k = (fs << 1) | fe;

			switch (k) { //intersecting
			case 0 : //Whole segment outside
				reject = true;
				break;
			case 1 : //PE
				if (inter && t>t_pe && t<=1.0f)
					t_pe = t;
				break;
			case 2 : //PL
				if (inter && t<t_pl && t>=0.0f)
					t_pl = t;
				break;
			}

			if (t_pl<t_pe) //in this case, the whole line is outside
				reject = true;
		}

		if (reject)
			continue;

		// Clone the start point, only if needed
		bool cloned = false;
		if (t_pe>0.0f && t_pl<1.0f) {              //means the start or end point will be modified, cloning needed
			copyVert(vs, &vsclone);
			cloned = true;
		}

		// Output vertices
		if (t_pe>0.0f)
			interpolateVert(vs, ve, t_pe, vout++);   //modify start point
		else
			copyVert(vs, vout++);                    //copy start point

		if (t_pl<1.0f)
			interpolateVert(cloned? &vsclone:vs, ve, t_pl, vout++); //modify end point
		else
			copyVert(ve, vout++);                    //copy end point

		cnt+=2;
	}

	return cnt;
}
//----------------------------------------------------------------------------
unsigned int CGPrimClipper::clip_poly_SH(CGVaryings *vinBuf, int vinCnt)
{
	// Clip against the Near first to avoid potential big scalars
	static const unsigned int clipOrder[ClipBound::Cnt] = {
		ClipBound::Near, ClipBound::Left,
		ClipBound::Right, ClipBound::Bottom,
		ClipBound::Top, ClipBound::Far
	};
	// Init out buffer
	CGVaryings *voutBuf = &sm_vout_buf[0], *tmp = nullptr;

	// Do clipping
	for (int i=0; i<ClipBound::Cnt; ++i) {
		if (! (vinCnt = clip_poly_SH_single(clipOrder[i], vinBuf, vinCnt, voutBuf)) ) {
			/* early exit, primitive is empty */
			return 0;
		}
		// Swap in and out buffers
		tmp = vinBuf;
		vinBuf = voutBuf;
		voutBuf = tmp;
	}

	// If some clipping has been done and the result was output to the original "vinBuf",
	// (in case of an odd swapping count), we need to copy the result back to the sm_voutBuf.
	// Note that the voutBuf is now pointing the sm_voutBuf, because of the unnecessary swap
	// after the last clipping.
	if (clip_bits && voutBuf == &sm_vout_buf[0])
		for (int j=0; j<vinCnt; ++j)
			copyVert(vinBuf+j, sm_vout_buf+j);

	return (vinCnt);
}
//----------------------------------------------------------------------------
int CGPrimClipper::clip_poly_SH_single(unsigned int clipBound, const CGVaryings *vinBuf, int nvin, CGVaryings *voutBuf)
{
	int voutCnt = 0;
	CGVaryings *vout = voutBuf;        //current output vertex
	float t=0.0f;                            //ratio for interpolation
	const CGVaryings *vs = nullptr, *ve = nullptr; //the line segment to be checked is always vs->ve
	const CGVec4 *ps = nullptr, *pe = nullptr;       //positions of vertices, for boundary test and intersection
	unsigned int fs, fe;                          //flags indicating that start and end points are inside or not

	//  Situation is encoded as start_in | end_in
	//  00 : Wholly outside visible region - save nothing
	//  01 : Enter visible region - save intersection and endpoint
	//  10 : Exit visible region - save the intersection
	//  11 : Wholly inside visible region - save endpoint
	unsigned int k = 0;
	static const unsigned int mask = 0x0003;

	// Initialize the info for start point
	vs = vinBuf+nvin-1; //the loop begin from the (n-1)-th vertex, to the 0-th vertex
	ps = &vs->position;
	fs = isInside(clipBound, *ps);
	k |= (fs << 1);     //construct the code

	for (int j=0; j<nvin; ++j) {
		// Get the info for the endpoint
		ve = vinBuf+j;
		pe = &ve->position;
		fe = isInside(clipBound, *pe);
		k |= fe;   //add the new right-most bit

		switch (k&mask) {
		case 1 : //Entering visible region
		{
			intersectLine(clipBound, ps, pe, t);
			interpolateVert(vs, ve, t, vout++);
			copyVert(ve, vout++);
			voutCnt+=2;
			break;
		}
		case 2 : //Exiting visible region
		{
			intersectLine(clipBound, ps, pe, t);
			interpolateVert(vs, ve, t, vout++);
			voutCnt++;
			break;
		}
		case 3 : //Whole segment inside
		{
			copyVert(ve, vout++);
			voutCnt++;
			break;
		}
		}

		// The end point becomes the new start point
		vs = ve;
		ps = pe;
		fs = fe;
		k <<= 1;  //shifting the right-most bit
	}

	return voutCnt;
}
//---------------------------------------------------------------------------
