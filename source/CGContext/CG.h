#pragma once

//---------------------------------------------------------------------------
// DEFINES
//---------------------------------------------------------------------------

/// Primitive types.
enum CGPrimitiveType
{
	CG_POINTS         = 0x0000,
	CG_LINES          = 0x0001,
	//CG_LINE_LOOP      = 0x0002,
	//CG_LINE_STRIP     = 0x0003,
	CG_TRIANGLES      = 0x0004,
	//CG_TRIANGLE_STRIP = 0x0005,
	//CG_TRIANGLE_FAN   = 0x0006,
	//CG_QUADS          = 0x0007,
	//CG_QUAD_STRIP     = 0x0008,
	//CG_POLYGON        = 0x0009,
};

/// Polygon modes
enum CGPolygonMode
{
	CG_LINE            = 0x1B01,
	CG_FILL            = 0x1B02
};

/// Texture filter modes
enum CGFilterMode
{
	CG_NEAREST         = 0x2600,
	CG_LINEAR          = 0x2601
};

/// Texture wrap modes
enum CGWrapMode
{
	CG_CLAMP           = 0x2900,
	CG_REPEAT          = 0x2901
};

/// Bit values for cgClear
#define CG_COLOR_BUFFER_BIT 0x00004000u
#define CG_DEPTH_BUFFER_BIT 0x00000100u

/// Limits
#define CG_MAX_VERTICES_IN_PIPELINE    32
//---------------------------------------------------------------------------

