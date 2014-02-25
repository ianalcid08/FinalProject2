/*
	File:		glsMath.cpp
	Author:		Bob Pasekoff
	Updated:	7/16/2013

*/

#include "../Header Files/stdafx.h"
#include "../Header Files/glsMath.h"

void glsLoadIdentity3x3( Matrix3x3f _m )
{
	// Don't be fooled, this is still column major
	static Matrix3x3f identity = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	memcpy( _m, identity, sizeof( Matrix3x3f ) );
}

void glsLoadIdentity3x3( Matrix3x3d _m )
{
	// Don't be fooled, this is still column major
	static Matrix3x3d identity = {
		1.0, 0.0, 0.0 ,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0
	};

	memcpy( _m, identity, sizeof( Matrix3x3d ) );
}

void glsLoadIdentity4x4( Matrix4x4f _m )
{
	// Don't be fooled, this is still column major
	static Matrix4x4f identity = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	memcpy( _m, identity, sizeof( Matrix4x4f ) );
}

void glsLoadIdentity4x4( Matrix4x4d _m )
{
	static Matrix4x4d identity = {
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	};

	memcpy( _m, identity, sizeof( Matrix4x4d ) );
}

float glsGetDistanceSquared3( const Vec3f _u, const Vec3f _v )
{
	float x = _u[0] - _v[0];
	x = x*x;

	float y = _u[1] - _v[1];
	y = y*y;

	float z = _u[2] - _v[2];
	z = z*z;

	return (x + y + z);
}

double glsGetDistanceSquared3( const Vec3d _u, const Vec3d _v )
{
	double x = _u[0] - _v[0];
	x = x*x;

	double y = _u[1] - _v[1];
	y = y*y;

	double z = _u[2] - _v[2];
	z = z*z;

	return (x + y + z);
}

#define A(row,col)  _a[(col<<2)+row]
#define B(row,col)  _b[(col<<2)+row]
#define P(row,col)  _product[(col<<2)+row]

void glsMatrixMultiply4x4( Matrix4x4f _product, const Matrix4x4f _a, const Matrix4x4f _b )
{
	for (int i = 0; i < 4; i++) {
		float ai0=A(i,0),  ai1=A(i,1),  ai2=A(i,2),  ai3=A(i,3);
		P(i,0) = ai0 * B(0,0) + ai1 * B(1,0) + ai2 * B(2,0) + ai3 * B(3,0);
		P(i,1) = ai0 * B(0,1) + ai1 * B(1,1) + ai2 * B(2,1) + ai3 * B(3,1);
		P(i,2) = ai0 * B(0,2) + ai1 * B(1,2) + ai2 * B(2,2) + ai3 * B(3,2);
		P(i,3) = ai0 * B(0,3) + ai1 * B(1,3) + ai2 * B(2,3) + ai3 * B(3,3);
	}
}

void glsMatrixMultiply4x4( Matrix4x4d _product, const Matrix4x4d _a, const Matrix4x4d _b )
{
	for (int i = 0; i < 4; i++) {
		double ai0=A(i,0),  ai1=A(i,1),  ai2=A(i,2),  ai3=A(i,3);
		P(i,0) = ai0 * B(0,0) + ai1 * B(1,0) + ai2 * B(2,0) + ai3 * B(3,0);
		P(i,1) = ai0 * B(0,1) + ai1 * B(1,1) + ai2 * B(2,1) + ai3 * B(3,1);
		P(i,2) = ai0 * B(0,2) + ai1 * B(1,2) + ai2 * B(2,2) + ai3 * B(3,2);
		P(i,3) = ai0 * B(0,3) + ai1 * B(1,3) + ai2 * B(2,3) + ai3 * B(3,3);
	}
}
#undef A
#undef B
#undef P

void glsVectorMultiply4( Vec4f _product, const Vec4f _a, const Vec4f _b )
{
	_product[0] = _a[0] * _b[0];
	_product[1] = _a[1] * _b[1];
	_product[2] = _a[2] * _b[2];
	_product[3] = _a[3] * _b[3];
}

void glsVectorMultiply4( Vec4d _product, const Vec4d _a, const Vec4d _b )
{
	_product[0] = _a[0] * _b[0];
	_product[1] = _a[1] * _b[1];
	_product[2] = _a[2] * _b[2];
	_product[3] = _a[3] * _b[3];
}

void glsVectorMultiply3( Vec3f _product, const Vec3f _a, const Vec3f _b )
{
	_product[0] = _a[0] * _b[0];
	_product[1] = _a[1] * _b[1];
	_product[2] = _a[2] * _b[2];
}

void glsVectorMultiply3( Vec3d _product, const Vec3d _a, const Vec3d _b )
{
	_product[0] = _a[0] * _b[0];
	_product[1] = _a[1] * _b[1];
	_product[2] = _a[2] * _b[2];
}

void glsVectorMultiply2( Vec2f _product, const Vec2f _a, const Vec2f _b )
{
	_product[0] = _a[0] * _b[0];
	_product[1] = _a[1] * _b[1];
}

void glsVectorMultiply2( Vec2d _product, const Vec2d _a, const Vec2d _b )
{
	_product[0] = _a[0] * _b[0];
	_product[1] = _a[1] * _b[1];
}

#define A3x3(row,col)  _a[(col*3)+row]
#define B3x3(row,col)  _b[(col*3)+row]
#define P3x3(row,col)  _product[(col*3)+row]

void glsMatrixMultiply3x3( Matrix3x3f _product, const Matrix3x3f _a, const Matrix3x3f _b )
{
	for (int i = 0; i < 3; i++) {
		float ai0=A3x3(i,0), ai1=A3x3(i,1),  ai2=A3x3(i,2);
		P3x3(i,0) = ai0 * B3x3(0,0) + ai1 * B3x3(1,0) + ai2 * B3x3(2,0);
		P3x3(i,1) = ai0 * B3x3(0,1) + ai1 * B3x3(1,1) + ai2 * B3x3(2,1);
		P3x3(i,2) = ai0 * B3x3(0,2) + ai1 * B3x3(1,2) + ai2 * B3x3(2,2);
	}
}

void glsMatrixMultiply3x3( Matrix3x3d _product, const Matrix3x3d _a, const Matrix3x3d _b )
{
	for (int i = 0; i < 3; i++) {
		double ai0=A3x3(i,0),  ai1=A3x3(i,1),  ai2=A3x3(i,2);
		P3x3(i,0) = ai0 * B3x3(0,0) + ai1 * B3x3(1,0) + ai2 * B3x3(2,0);
		P3x3(i,1) = ai0 * B3x3(0,1) + ai1 * B3x3(1,1) + ai2 * B3x3(2,1);
		P3x3(i,2) = ai0 * B3x3(0,2) + ai1 * B3x3(1,2) + ai2 * B3x3(2,2);
	}
}

#undef A3x3
#undef B3x3
#undef P3x3

void glsMakePerspectiveMatrix( Matrix4x4f _mProjection, float _fFov, float _fAspect, float _zMin, float _zMax )
{
	glsLoadIdentity4x4( _mProjection ); // Fastest way to get most valid values already in place

	float yMax = _zMin * tanf( _fFov * 0.5f );
	float yMin = -yMax;
	float xMin = yMin * _fAspect;
	float xMax = -xMin; 

	_mProjection[0] = (2.0f * _zMin) / (xMax - xMin);
	_mProjection[5] = (2.0f * _zMin) / (yMax - yMin);
	_mProjection[8] = (xMax + xMin) / (xMax - xMin);
	_mProjection[9] = (yMax + yMin) / (yMax - yMin);
	_mProjection[10] = -((_zMax + _zMin) / (_zMax - _zMin));
	_mProjection[11] = -1.0f;
	_mProjection[14] = -((2.0f * (_zMax * _zMin))/(_zMax - _zMin));
	_mProjection[15] = 0.0f;
}

void glsMakeOrthographicMatrix( Matrix4x4f _mProjection, float _xMin, float _xMax, float _yMin, float _yMax, float _zMin, float _zMax)
{
	glsLoadIdentity4x4( _mProjection );

	if(_xMax - _xMin == 0.0f || _yMax - _yMin == 0.0f || _zMax - _zMin == 0.0f )
		return;

	_mProjection[0] = 2.0f / (_xMax - _xMin);
	_mProjection[5] = 2.0f / (_yMax - _yMin);
	_mProjection[10] = -2.0f / (_zMax - _zMin);
	_mProjection[12] = -((_xMax + _xMin)/(_xMax - _xMin));
	_mProjection[13] = -((_yMax + _yMin)/(_yMax - _yMin));
	_mProjection[14] = -((_zMax + _zMin)/(_zMax - _zMin));
	_mProjection[15] = 1.0f;
}

#define M3x3(row,col)  _m[col*3+row]

void glsRotationMatrix3x3( Matrix3x3f _m, float _angle, float _x, float _y, float _z )
{
	float mag, s, c;
	float xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;

	s = float(sin( _angle ));
	c = float(cos( _angle ));

	mag = float(sqrt( _x * _x + _y * _y + _z * _z ));

	// Identity matrix
	if (mag == 0.0f) {
		glsLoadIdentity3x3( _m );
		return;
	}

	// Rotation matrix is normalized
	_x /= mag;
	_y /= mag;
	_z /= mag;

	xx = _x * _x;
	yy = _y * _y;
	zz = _z * _z;
	xy = _x * _y;
	yz = _y * _z;
	zx = _z * _x;
	xs = _x * s;
	ys = _y * s;
	zs = _z * s;
	one_c = 1.0f - c;

	M3x3(0,0) = (one_c * xx) + c;
	M3x3(0,1) = (one_c * xy) - zs;
	M3x3(0,2) = (one_c * zx) + ys;

	M3x3(1,0) = (one_c * xy) + zs;
	M3x3(1,1) = (one_c * yy) + c;
	M3x3(1,2) = (one_c * yz) - xs;

	M3x3(2,0) = (one_c * zx) - ys;
	M3x3(2,1) = (one_c * yz) + xs;
	M3x3(2,2) = (one_c * zz) + c;
}

#undef M3x3

void glsRotationMatrix4x4( Matrix4x4f _m, float _angle, float _x, float _y, float _z )
{
	float mag, s, c;
	float xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;

	s = float(sin( _angle) );
	c = float(cos( _angle) );

	mag = float(sqrt( _x * _x + _y * _y + _z * _z ));

	// Identity matrix
	if ( mag == 0.0f )
	{
		glsLoadIdentity4x4( _m );
		return;
	}

	// Rotation matrix is normalized
	_x /= mag;
	_y /= mag;
	_z /= mag;

#define M(row,col)  _m[col * 4 + row]

	xx = _x * _x;
	yy = _y * _y;
	zz = _z * _z;
	xy = _x * _y;
	yz = _y * _z;
	zx = _z * _x;
	xs = _x * s;
	ys = _y * s;
	zs = _z * s;
	one_c = 1.0f - c;

	M(0,0) = (one_c * xx) + c;
	M(0,1) = (one_c * xy) - zs;
	M(0,2) = (one_c * zx) + ys;
	M(0,3) = 0.0f;

	M(1,0) = (one_c * xy) + zs;
	M(1,1) = (one_c * yy) + c;
	M(1,2) = (one_c * yz) - xs;
	M(1,3) = 0.0f;

	M(2,0) = (one_c * zx) - ys;
	M(2,1) = (one_c * yz) + xs;
	M(2,2) = (one_c * zz) + c;
	M(2,3) = 0.0f;

	M(3,0) = 0.0f;
	M(3,1) = 0.0f;
	M(3,2) = 0.0f;
	M(3,3) = 1.0f;

#undef M
}

void glsRotationMatrix3x3( Matrix3x3d _m, double _angle, double _x, double _y, double _z )
{
	double mag, s, c;
	double xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;

	s = sin( _angle );
	c = cos( _angle );

	mag = sqrt( _x * _x + _y * _y + _z * _z );

	// Identity matrix
	if (mag == 0.0)
	{
		glsLoadIdentity3x3( _m );
		return;
	}

	// Rotation matrix is normalized
	_x /= mag;
	_y /= mag;
	_z /= mag;

#define M(row,col)  _m[col*3+row]

	xx = _x * _x;
	yy = _y * _y;
	zz = _z * _z;
	xy = _x * _y;
	yz = _y * _z;
	zx = _z * _x;
	xs = _x * s;
	ys = _y * s;
	zs = _z * s;
	one_c = 1.0 - c;

	M(0,0) = (one_c * xx) + c;
	M(0,1) = (one_c * xy) - zs;
	M(0,2) = (one_c * zx) + ys;

	M(1,0) = (one_c * xy) + zs;
	M(1,1) = (one_c * yy) + c;
	M(1,2) = (one_c * yz) - xs;

	M(2,0) = (one_c * zx) - ys;
	M(2,1) = (one_c * yz) + xs;
	M(2,2) = (one_c * zz) + c;

#undef M
}

void glsRotationMatrix4x4( Matrix4x4d _m, double _angle, double _x, double _y, double _z )
{
	double mag, s, c;
	double xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;

	s = sin( _angle );
	c = cos( _angle );

	mag = sqrt( _x * _x + _y * _y + _z * _z );

	// Identity matrix
	if (mag == 0.0)
	{
		glsLoadIdentity4x4( _m );
		return;
	}

	// Rotation matrix is normalized
	_x /= mag;
	_y /= mag;
	_z /= mag;

#define M(row,col)  _m[col*4+row]

	xx = _x * _x;
	yy = _y * _y;
	zz = _z * _z;
	xy = _x * _y;
	yz = _y * _z;
	zx = _z * _x;
	xs = _x * s;
	ys = _y * s;
	zs = _z * s;
	one_c = 1.0f - c;

	M(0,0) = (one_c * xx) + c;
	M(0,1) = (one_c * xy) - zs;
	M(0,2) = (one_c * zx) + ys;
	M(0,3) = 0.0;

	M(1,0) = (one_c * xy) + zs;
	M(1,1) = (one_c * yy) + c;
	M(1,2) = (one_c * yz) - xs;
	M(1,3) = 0.0;

	M(2,0) = (one_c * zx) - ys;
	M(2,1) = (one_c * yz) + xs;
	M(2,2) = (one_c * zz) + c;
	M(2,3) = 0.0;

	M(3,0) = 0.0;
	M(3,1) = 0.0;
	M(3,2) = 0.0;
	M(3,3) = 1.0;

#undef M
}

static float DetIJ( const Matrix4x4f _m, const int _i, const int _j )
{
	int x, y, ii, jj;
	float ret, mat[3][3];

	x = 0;
	for (ii = 0; ii < 4; ii++)
	{
		if (ii == _i) continue;
		y = 0;
		for (jj = 0; jj < 4; jj++)
		{
			if (jj == _j) continue;
			mat[x][y] = _m[(ii*4)+jj];
			y++;
		}
		x++;
	}

	ret =  mat[0][0]*(mat[1][1]*mat[2][2]-mat[2][1]*mat[1][2]);
	ret -= mat[0][1]*(mat[1][0]*mat[2][2]-mat[2][0]*mat[1][2]);
	ret += mat[0][2]*(mat[1][0]*mat[2][1]-mat[2][0]*mat[1][1]);

	return ret;
}

static double DetIJ( const Matrix4x4d _m, const int _i, const int _j )
{
	int x, y, ii, jj;
	double ret, mat[3][3];

	x = 0;
	for (ii = 0; ii < 4; ii++)
	{
		if (ii == _i) continue;
		y = 0;
		for (jj = 0; jj < 4; jj++)
		{
			if (jj == _j) continue;
			mat[x][y] = _m[(ii*4)+jj];
			y++;
		}
		x++;
	}

	ret =  mat[0][0]*(mat[1][1]*mat[2][2]-mat[2][1]*mat[1][2]);
	ret -= mat[0][1]*(mat[1][0]*mat[2][2]-mat[2][0]*mat[1][2]);
	ret += mat[0][2]*(mat[1][0]*mat[2][1]-mat[2][0]*mat[1][1]);

	return ret;
}

void glsInvertMatrix4x4( Matrix4x4f _mInverse, const Matrix4x4f _m )
{
	int i, j;
	float det, detij;

	// calculate 4x4 determinant
	det = 0.0f;
	for (i = 0; i < 4; i++)
	{
		det += (i & 0x1) ? (-_m[i] * DetIJ( _m, 0, i )) : (_m[i] * DetIJ( _m, 0, i ));
	}
	det = 1.0f / det;

	// calculate inverse
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			detij = DetIJ( _m, j, i );
			_mInverse[(i*4)+j] = ((i+j) & 0x1) ? (-detij * det) : (detij *det); 
		}
	}
}

void glsInvertMatrix4x4( Matrix4x4d _mInverse, const Matrix4x4d _m)
{
	int i, j;
	double det, detij;

	// calculate 4x4 determinant
	det = 0.0;
	for (i = 0; i < 4; i++)
	{
		det += (i & 0x1) ? (-_m[i] * DetIJ( _m, 0, i )) : (_m[i] * DetIJ( _m, 0,i ));
	}
	det = 1.0 / det;

	// calculate inverse
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			detij = DetIJ( _m, j, i );
			_mInverse[(i*4)+j] = ((i+j) & 0x1) ? (-detij * det) : (detij *det); 
		}
	}
}

void glsTransposeMatrix4x4( Matrix4x4f _mTranspose, const Matrix4x4f _m )
{
	_mTranspose[0] = _m[0];
	_mTranspose[1] = _m[4];
	_mTranspose[2] = _m[8];
	_mTranspose[3] = _m[12];

	_mTranspose[4] = _m[1];
	_mTranspose[5] = _m[5];
	_mTranspose[6] = _m[9];
	_mTranspose[7] = _m[13];

	_mTranspose[8] = _m[2];
	_mTranspose[9] = _m[6];
	_mTranspose[10] = _m[10];
	_mTranspose[11] = _m[14];

	_mTranspose[12] = _m[3];
	_mTranspose[13] = _m[7];
	_mTranspose[14] = _m[11];
	_mTranspose[15] = _m[15];
}

void glsTransposeMatrix4x4( Matrix4x4d _mTranspose, const Matrix4x4d _m )
{
	_mTranspose[0] = _m[0];
	_mTranspose[1] = _m[4];
	_mTranspose[2] = _m[8];
	_mTranspose[3] = _m[12];

	_mTranspose[4] = _m[1];
	_mTranspose[5] = _m[5];
	_mTranspose[6] = _m[9];
	_mTranspose[7] = _m[13];

	_mTranspose[8] = _m[2];
	_mTranspose[9] = _m[6];
	_mTranspose[10] = _m[10];
	_mTranspose[11] = _m[14];

	_mTranspose[12] = _m[3];
	_mTranspose[13] = _m[7];
	_mTranspose[14] = _m[11];
	_mTranspose[15] = _m[15];
}

// Get Window coordinates, discard Z...
void glsProjectXY( Vec2f _vPointOut, const Matrix4x4f _mModelView, const Matrix4x4f _mProjection, const int _iViewPort[4], const Vec3f _vPointIn )
{
	Vec4f vBack, vForth;

	memcpy( vBack, _vPointIn, sizeof( float ) * 3 );
	vBack[3] = 1.0f;

	glsTransformVector4( vForth, vBack, _mModelView );
	glsTransformVector4( vBack, vForth, _mProjection );

	if( !glsCloseEnough( vBack[3], 0.0f, 0.000001f ) )
	{
		float div = 1.0f / vBack[3];
		vBack[0] *= div;
		vBack[1] *= div;
		//vBack[2] *= div; 
	}

	_vPointOut[0] = float(_iViewPort[0])+(1.0f+float(vBack[0]))*float(_iViewPort[2])/2.0f;
	_vPointOut[1] = float(_iViewPort[1])+(1.0f+float(vBack[1]))*float(_iViewPort[3])/2.0f;

	// This was put in for Grand Tour... I think it's right. 
	// .... please report any bugs
	if( _iViewPort[0] != 0 )     // Cast to float is expensive... avoid if posssible
		_vPointOut[0] -= float(_iViewPort[0]);

	if( _iViewPort[1] != 0 ) 
		_vPointOut[1] -= float(_iViewPort[1]);
}

// Get window coordinates, we also want Z....
void glsProjectXYZ( Vec3f _vPointOut, const Matrix4x4f _mModelView, const Matrix4x4f _mProjection, const int _iViewPort[4], const Vec3f _vPointIn )
{
	Vec4f vBack, vForth;

	memcpy( vBack, _vPointIn, sizeof( float ) * 3 );
	vBack[3] = 1.0f;

	glsTransformVector4( vForth, vBack, _mModelView );
	glsTransformVector4( vBack, vForth, _mProjection );

	if( !glsCloseEnough( vBack[3], 0.0f, 0.000001f ) )
	{
		float div = 1.0f / vBack[3];
		vBack[0] *= div;
		vBack[1] *= div;
		vBack[2] *= div; 
	}

	_vPointOut[0] = float(_iViewPort[0])+(1.0f+float(vBack[0]))*float(_iViewPort[2])/2.0f;
	_vPointOut[1] = float(_iViewPort[1])+(1.0f+float(vBack[1]))*float(_iViewPort[3])/2.0f;

	if( _iViewPort[0] != 0 )     // Cast to float is expensive... avoid if posssible
		_vPointOut[0] -= float(_iViewPort[0]);

	if( _iViewPort[1] != 0 ) 
		_vPointOut[1] -= float(_iViewPort[1]);

	_vPointOut[2] = vBack[2];
}

// Calculates the normal of a triangle specified by the three points
// p1, p2, and p3.  The triangle is assumed to be wound counter clockwise. 
void glsFindNormal( Vec3f _result, const Vec3f _point1, const Vec3f _point2, const Vec3f _point3 )
{
	Vec3f v1,v2;		// Temporary vectors

	// Calculate two vectors from the three points. Assumes counter clockwise
	// winding!
	v1[0] = _point1[0] - _point2[0];
	v1[1] = _point1[1] - _point2[1];
	v1[2] = _point1[2] - _point2[2];

	v2[0] = _point2[0] - _point3[0];
	v2[1] = _point2[1] - _point3[1];
	v2[2] = _point2[2] - _point3[2];

	// Take the cross product of the two vectors to get
	// the normal vector.
	glsCrossProduct3( _result, v1, v2 );
}

void glsFindNormal( Vec3d _result, const Vec3d _point1, const Vec3d _point2, const Vec3d _point3 )
{
	Vec3d v1,v2;		// Temporary vectors

	// Calculate two vectors from the three points. Assumes counter clockwise
	// winding!
	v1[0] = _point1[0] - _point2[0];
	v1[1] = _point1[1] - _point2[1];
	v1[2] = _point1[2] - _point2[2];

	v2[0] = _point2[0] - _point3[0];
	v2[1] = _point2[1] - _point3[1];
	v2[2] = _point2[2] - _point3[2];

	// Take the cross product of the two vectors to get
	// the normal vector.
	glsCrossProduct3( _result, v1, v2 );
}

// points are given in clockwise winding order, with normal pointing out of clockwise face
// planeEq contains the A,B,C, and D of the plane equation coefficients
void glsGetPlaneEquation( Vec4f _planeEq, const Vec3f _p1, const Vec3f _p2, const Vec3f _p3 )
{
	Vec3f v1, v2;

	// V1 = p3 - p1
	v1[0] = _p3[0] - _p1[0];
	v1[1] = _p3[1] - _p1[1];
	v1[2] = _p3[2] - _p1[2];

	// V2 = P2 - p1
	v2[0] = _p2[0] - _p1[0];
	v2[1] = _p2[1] - _p1[1];
	v2[2] = _p2[2] - _p1[2];

	// Unit normal to plane
	glsCrossProduct3( _planeEq, v1, v2 );
	glsNormalizeVector3( _planeEq );

	// Back substitute to get D
	_planeEq[3] = -(_planeEq[0] * _p3[0] + _planeEq[1] * _p3[1] + _planeEq[2] * _p3[2] );
}

void glsGetPlaneEquation( Vec4d _planeEq, const Vec3d _p1, const Vec3d _p2, const Vec3d _p3 )
{
	Vec3d v1, v2;

	// V1 = p3 - p1
	v1[0] = _p3[0] - _p1[0];
	v1[1] = _p3[1] - _p1[1];
	v1[2] = _p3[2] - _p1[2];

	// V2 = P2 - p1
	v2[0] = _p2[0] - _p1[0];
	v2[1] = _p2[1] - _p1[1];
	v2[2] = _p2[2] - _p1[2];

	// Unit normal to plane
	glsCrossProduct3( _planeEq, v1, v2 );
	glsNormalizeVector3( _planeEq );
	// Back substitute to get D
	_planeEq[3] = -(_planeEq[0] * _p3[0] + _planeEq[1] * _p3[1] + _planeEq[2] * _p3[2] );
}

void glsCatmullRom( Vec3f _vOut, const Vec3f _vP0, const Vec3f _vP1, const Vec3f _vP2, const Vec3f _vP3, float _t )
{
	float t2 = _t * _t;
	float t3 = t2 * _t;

	// X    
	_vOut[0] = 0.5f * ( ( 2.0f * _vP1[0]) +
		(-_vP0[0] + _vP2[0]) * _t +
		(2.0f * _vP0[0] - 5.0f * _vP1[0] + 4.0f * _vP2[0] - _vP3[0]) * t2 +
		(-_vP0[0] + 3.0f * _vP1[0] - 3.0f * _vP2[0] + _vP3[0]) * t3);
	// Y
	_vOut[1] = 0.5f * ( ( 2.0f * _vP1[1]) +
		(-_vP0[1] + _vP2[1]) * _t +
		(2.0f * _vP0[1] - 5.0f * _vP1[1] + 4.0f * _vP2[1] - _vP3[1]) * t2 +
		(-_vP0[1] + 3.0f * _vP1[1] - 3.0f * _vP2[1] + _vP3[1]) * t3);

	// Z
	_vOut[2] = 0.5f * ( ( 2.0f * _vP1[2]) +
		(-_vP0[2] + _vP2[2]) * _t +
		(2.0f * _vP0[2] - 5.0f * _vP1[2] + 4.0f * _vP2[2] - _vP3[2]) * t2 +
		(-_vP0[2] + 3.0f * _vP1[2] - 3.0f * _vP2[2] + _vP3[2]) * t3);
}

void glsCatmullRom( Vec3d _vOut, const Vec3d _vP0, const Vec3d _vP1, const Vec3d _vP2, const Vec3d _vP3, double _t )
{
	double t2 = _t * _t;
	double t3 = t2 * _t;

	// X    
	_vOut[0] = 0.5f * ( ( 2.0f * _vP1[0]) +
		(-_vP0[0] + _vP2[0]) * _t +
		(2.0f * _vP0[0] - 5.0f * _vP1[0] + 4.0f * _vP2[0] - _vP3[0]) * t2 +
		(-_vP0[0] + 3.0f * _vP1[0] - 3.0f * _vP2[0] + _vP3[0]) * t3);
	// Y
	_vOut[1] = 0.5f * ( ( 2.0f * _vP1[1]) +
		(-_vP0[1] + _vP2[1]) * _t +
		(2.0f * _vP0[1] - 5.0f * _vP1[1] + 4.0f * _vP2[1] - _vP3[1]) * t2 +
		(-_vP0[1] + 3.0f * _vP1[1] - 3.0f * _vP2[1] + _vP3[1]) * t3);

	// Z
	_vOut[2] = 0.5f * ( ( 2.0f * _vP1[2]) +
		(-_vP0[2] + _vP2[2]) * _t +
		(2.0f * _vP0[2] - 5.0f * _vP1[2] + 4.0f * _vP2[2] - _vP3[2]) * t2 +
		(-_vP0[2] + 3.0f * _vP1[2] - 3.0f * _vP2[2] + _vP3[2]) * t3);
}

double glsRaySphereTest( const Vec3d _point, const Vec3d _ray, const Vec3d _sphereCenter, double _sphereRadius )
{
	Vec3d rayToCenter;	// Ray to center of sphere
	rayToCenter[0] =  _sphereCenter[0] - _point[0];	
	rayToCenter[1] =  _sphereCenter[1] - _point[1];
	rayToCenter[2] =  _sphereCenter[2] - _point[2];

	// Project rayToCenter on ray to test
	double a = glsDotProduct3( rayToCenter, _ray );

	// Distance to center of sphere
	double distance2 = glsDotProduct3( rayToCenter, rayToCenter );	// Or length


	double dRet = (_sphereRadius * _sphereRadius) - distance2 + (a * a);

	if( dRet > 0.0 )			// Return distance to intersection
		dRet = a - sqrt( dRet );

	return dRet;
}

float glsRaySphereTest( const Vec3f _point, const Vec3f _ray, const Vec3f _sphereCenter, float _sphereRadius )
{
	Vec3f rayToCenter;	// Ray to center of sphere
	rayToCenter[0] =  _sphereCenter[0] - _point[0];	
	rayToCenter[1] =  _sphereCenter[1] - _point[1];
	rayToCenter[2] =  _sphereCenter[2] - _point[2];

	// Project rayToCenter on ray to test
	float a = glsDotProduct3( rayToCenter, _ray );

	// Distance to center of sphere
	float distance2 = glsDotProduct3( rayToCenter, rayToCenter );	// Or length


	float dRet = (_sphereRadius * _sphereRadius) - distance2 + (a * a);

	if( dRet > 0.0 )			// Return distance to intersection
		dRet = a - sqrtf( dRet );

	return dRet;
}

void glsCalculateTangentBasis( Vec3f _vTangent, const Vec3f _vTriangle[3], const Vec2f _vTexCoords[3], const Vec3f _N )
{
	Vec3f dv2v1, dv3v1;
	float dc2c1t, dc2c1b, dc3c1t, dc3c1b;
	float M;

	glsSubtractVectors3( dv2v1, _vTriangle[1], _vTriangle[0]);
	glsSubtractVectors3( dv3v1, _vTriangle[2], _vTriangle[0]);

	dc2c1t = _vTexCoords[1][0] - _vTexCoords[0][0];
	dc2c1b = _vTexCoords[1][1] - _vTexCoords[0][1];
	dc3c1t = _vTexCoords[2][0] - _vTexCoords[0][0];
	dc3c1b = _vTexCoords[2][1] - _vTexCoords[0][1];

	M = (dc2c1t * dc3c1b) - (dc3c1t * dc2c1b);
	M = 1.0f / M;

	glsScaleVector3( dv2v1, dc3c1b );
	glsScaleVector3( dv3v1, dc2c1b );

	glsSubtractVectors3( _vTangent, dv2v1, dv3v1 );
	glsScaleVector3( _vTangent, M );  // This potentially changes the direction of the vector
	glsNormalizeVector3( _vTangent );

	Vec3f B;
	glsCrossProduct3( B, _N, _vTangent );
	glsCrossProduct3( _vTangent, B, _N );
	glsNormalizeVector3( _vTangent );
}

double glsSmoothStep( const double _edge1, const double _edge2, const double _x )
{
	if( _edge2 - _edge1 == 0.0 )
		return 0.0;

	double t;
	t = (_x - _edge1) / (_edge2 - _edge1);
	if( t > 1.0 )
		t = 1.0;

	if( t < 0.0 )
		t = 0.0f;

	return t * t * ( 3.0 - 2.0 * t);
}

float glsSmoothStep( const float _edge1, const float _edge2, const float _x )
{

	if( _edge2 - _edge1 == 0.0f )
		return 0.0f;

	float t;
	t = (_x - _edge1) / (_edge2 - _edge1);
	if( t > 1.0f )
		t = 1.0f;

	if( t < 0.0f )
		t = 0.0f;

	return t * t * ( 3.0f - 2.0f * t);
}

void glsMakePlanarShadowMatrix( Matrix4x4f _proj, const Vec4f _planeEq, const Vec3f _vLightPos )
{	
	float a = _planeEq[0];
	float b = _planeEq[1];
	float c = _planeEq[2];
	float d = _planeEq[3];

	float dx = -_vLightPos[0];
	float dy = -_vLightPos[1];
	float dz = -_vLightPos[2];

	_proj[0] = b * dy + c * dz;
	_proj[1] = -a * dy;
	_proj[2] = -a * dz;
	_proj[3] = 0.0;

	_proj[4] = -b * dx;
	_proj[5] = a * dx + c * dz;
	_proj[6] = -b * dz;
	_proj[7] = 0.0;

	_proj[8] = -c * dx;
	_proj[9] = -c * dy;
	_proj[10] = a * dx + b * dy;
	_proj[11] = 0.0;

	_proj[12] = -d * dx;
	_proj[13] = -d * dy;
	_proj[14] = -d * dz;
	_proj[15] = a * dx + b * dy + c * dz;
}

void glsMakePlanarShadowMatrix( Matrix4x4d _proj, const Vec4d _planeEq, const Vec3d _vLightPos )
{	
	double a = _planeEq[0];
	double b = _planeEq[1];
	double c = _planeEq[2];
	double d = _planeEq[3];

	double dx = -_vLightPos[0];
	double dy = -_vLightPos[1];
	double dz = -_vLightPos[2];

	_proj[0] = b * dy + c * dz;
	_proj[1] = -a * dy;
	_proj[2] = -a * dz;
	_proj[3] = 0.0;

	_proj[4] = -b * dx;
	_proj[5] = a * dx + c * dz;
	_proj[6] = -b * dz;
	_proj[7] = 0.0;

	_proj[8] = -c * dx;
	_proj[9] = -c * dy;
	_proj[10] = a * dx + b * dy;
	_proj[11] = 0.0;

	_proj[12] = -d * dx;
	_proj[13] = -d * dy;
	_proj[14] = -d * dz;
	_proj[15] = a * dx + b * dy + c * dz;
}

double glsClosestPointOnRay( Vec3d _vPointOnRay, const Vec3d _vRayOrigin, const Vec3d _vUnitRayDir, const Vec3d _vPointInSpace )
{
	Vec3d v;
	glsSubtractVectors3( v, _vPointInSpace, _vRayOrigin );

	double t = glsDotProduct3( _vUnitRayDir, v );

	// This is the point on the ray
	_vPointOnRay[0] = _vRayOrigin[0] + (t * _vUnitRayDir[0]);
	_vPointOnRay[1] = _vRayOrigin[1] + (t * _vUnitRayDir[1]);
	_vPointOnRay[2] = _vRayOrigin[2] + (t * _vUnitRayDir[2]);

	return glsGetDistanceSquared3( _vPointOnRay, _vPointInSpace );
}

float glsClosestPointOnRay( Vec3f _vPointOnRay, const Vec3f _vRayOrigin, const Vec3f _vUnitRayDir, const Vec3f _vPointInSpace )
{
	Vec3f v;
	glsSubtractVectors3( v, _vPointInSpace, _vRayOrigin );

	float t = glsDotProduct3( _vUnitRayDir, v );

	// This is the point on the ray
	_vPointOnRay[0] = _vRayOrigin[0] + (t * _vUnitRayDir[0]);
	_vPointOnRay[1] = _vRayOrigin[1] + (t * _vUnitRayDir[1]);
	_vPointOnRay[2] = _vRayOrigin[2] + (t * _vUnitRayDir[2]);

	return glsGetDistanceSquared3( _vPointOnRay, _vPointInSpace );
}


void glsGetVec3FromMat4( Vec3f _VecToFillOut, Matrix4x4f _Mat, int _WhichVec )
{
	switch(_WhichVec)
	{
	case 0:
		{
			_VecToFillOut[0] = _Mat[0];
			_VecToFillOut[1] = _Mat[1];
			_VecToFillOut[2] = _Mat[2];
		}
		break;

	case 1:
		{
			_VecToFillOut[0] = _Mat[4];
			_VecToFillOut[1] = _Mat[5];
			_VecToFillOut[2] = _Mat[6];
		}
		break;

	case 2:
		{
			_VecToFillOut[0] = _Mat[8];
			_VecToFillOut[1] = _Mat[9];
			_VecToFillOut[2] = _Mat[10];
		}
		break;

	case 3:
		{
			_VecToFillOut[0] = _Mat[12];
			_VecToFillOut[1] = _Mat[13];
			_VecToFillOut[2] = _Mat[14];
		}
		break;
	}
}


void glsLookAt( Matrix4x4f _Source, Matrix4x4f _Destination )
{
	Vec3f WorldUp;
	WorldUp[0] = 0;
	WorldUp[1] = 1;
	WorldUp[2] = 0;

	Vec3f vTarget; glsGetVec3FromMat4(vTarget, _Destination, 3);
	Vec3f vSource; glsGetVec3FromMat4(vSource, _Source, 3);

	Vec3f vOrientation; glsSubtractVectors3(vOrientation, vTarget, vSource);

	Vec3f vZAxis; glsCopyVector3(vZAxis, vOrientation);

	glsNormalizeVector3(vZAxis);

	Vec3f vXAxis;
	glsCrossProduct3(vXAxis, WorldUp , vZAxis);

	glsNormalizeVector3( vXAxis );

	Vec3f vYAxis;
	glsCrossProduct3(vYAxis, vZAxis, vXAxis);
	glsNormalizeVector3( vYAxis);

	//mm.XAxis = xAxis;
	_Source[0] = vXAxis[0];
	_Source[1] = vXAxis[1];
	_Source[2] = vXAxis[2];
	_Source[3] = 0;

	//mm.YAxis = yAxis;
	_Source[4] = vYAxis[0];
	_Source[5] = vYAxis[1];
	_Source[6] = vYAxis[2];
	_Source[7] = 0;


	//mm.ZAxis = zAxis;
	_Source[8]  = vZAxis[0];
	_Source[9]  = vZAxis[1];
	_Source[10] = vZAxis[2];
	_Source[11] = 0;


	//mm.WAxis = turnToPtr->gameObject->GetLocalTransform().WAxis;

	_Source[12] = vSource[0];
	_Source[13] = vSource[1];
	_Source[14] = vSource[2];
	_Source[15] = 1.0f;

}
