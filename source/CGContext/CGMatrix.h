#pragma once
#include <cstdlib>
#include <cassert>
#include "CG.h"
// Forward declaration
struct CGVec4;

class CGMatrix4x4
{
public:
	// --- static members for constructing single transformation ---

	/// returns an instance of the identity matrix.
	static CGMatrix4x4 getIdentityMatrix();

	/// returns an instance of a translation matrix.
	static CGMatrix4x4 getTranslationMatrix(float x, float y, float z);

	/// returns an instance of a rotation matrix around the X axis
	/// rotation has to be in the _mathematically_ correct sense
	/// angle is in degrees
	static CGMatrix4x4 getRotationMatrixX(float angle);

	/// returns an instance of a rotation matrix around the Y axis
	/// rotation has to be in the _mathematically_ correct sense
	/// angle is in degrees
	static CGMatrix4x4 getRotationMatrixY(float angle);

	/// returns an instance of a rotation matrix around the Z axis
	/// rotation has to be in the _mathematically_ correct sense
	/// angle is in degrees
	static CGMatrix4x4 getRotationMatrixZ(float angle);

	/// returns an instance of a scale matrix
	static CGMatrix4x4 getScaleMatrix(float x, float y, float z);

	/// returns an instance of a rotation matrix around an arbitrary axis
	/// angle is in degrees
	static CGMatrix4x4 getRotationMatrix(float angle, float x, float y, float z);

	///  returns a perspective projection matrix for a specified frustum
	static CGMatrix4x4 getFrustum(float left, float right,
	                              float bottom, float top,
	                              float nearVal, float farVal);

	///  returns an orthogonal projection matrix for a specified frustum
	static CGMatrix4x4 getOrtho(float left, float right,
	                            float bottom, float top,
	                            float nearVal, float farVal);

	/// returns a symmetrical perspecitive matrix for the specified parameters
	static CGMatrix4x4 getPerspective(float fov_y, float aspect,
	                                  float nearVal, float farVal);

	/// retruns a view matrix for the specified camera position and orientation
	static CGMatrix4x4 getLookAt(const CGVec4& eye,
	                             const CGVec4& center,
	                             const CGVec4 up);

	// --- non-static members for modifying the current matrix (this) ---

	// Hint 1: you can use *this = ... to assign the whole matrix.
	// Hint 2: use the static members and the multiplication operator whenever possible.

	/// constructor initializes the matrix as any diagonal matrix
	/// (default: identity matrix)
	CGMatrix4x4(const float d = 1.0f);

	/// set the c'th column of the matrix
	void setColumn(const int c, const CGVec4& v);

	/// set the r'th row of the matrix
	void setRow(const int r, const CGVec4& v);

	/// set the matrix to Identity
	void identity();

	/// transpose the matrix
	void transpose();

	/// invert the matrix
	void invert();

	/// print the matrix to the console (stdout/cout)
	/// prints the matrix as 4 lines with 4 columns each,
	/// separated by a whitespace and aligned column-wise
	/// each line is optionally prefixed with a string
	void print(const char *prefix=nullptr) const;

	/// access the entry at the c'th column and the r'th row of the matrix
	/// (read and write)
	float& at(const unsigned int c,
	          const unsigned int r)
	{
		assert(c<4&&r<4);
		return m_matrix[c][r];
	}

	/// access the entry at the c'th column and the r'th row of the matrix
	/// (read only)
	const float& at(const unsigned int c, const unsigned int r) const
	{
		assert(c<4&&r<4);
		return m_matrix[c][r];
	}

	/// multiply two matrices, return result as new matrix
	CGMatrix4x4 operator* (const CGMatrix4x4& m) const;

	/// multiply matrix with vector, return result as new vector
	CGVec4 operator* (const CGVec4& v) const;

	/// retrieve pointer to the float array
	const float * getDataPtr() const {return &m_matrix[0][0];}
private:
	/// the real matrix data
	float m_matrix[4][4];
};

