/*
	File:		glsMath.h
	Author:		Bob Pasekoff
	Updated:	7/16/2013

*/

#pragma once

#include <math.h>
#include <string.h>

typedef float	Vec2f[2];
typedef double	Vec2d[2];

typedef float	Vec3f[3];
typedef double	Vec3d[3];

typedef float	Vec4f[4];
typedef double	Vec4d[5];


// 3x3 matrix, column major
typedef float	Matrix3x3f[9];
typedef double	Matrix3x3d[9];

// 4x4 matrix, column major
typedef float	Matrix4x4f[16];
typedef double	Matrix4x4d[16];

// some constants
#define GLS_PI (3.14159265358979323846)
#define GLS_2PI (2.0 * GLS_PI)
#define GLS_PI_DIV_180 (0.017453292519943296)
#define GLS_INV_PI_DIV_180 (57.2957795130823229)

// some shortcuts
#define glsDegToRad( x )	((x) * GLS_PI_DIV_180)
#define glsRadToDeg( x )	((x) * GLS_INV_PI_DIV_180)

// Hour angles
#define glsHrToDeg( x )	((x) * (1.0 / 15.0))
#define glsHrToRad( x )	glsDegToRad( glsHrToDeg( x ) )

#define glsDegToHr( x )	((x) * 15.0))
#define glsRadToHr( x )	glsDegToHr( glsRadToDeg( x ) )

/*
	Returns the same number if it is a power of
	two. Returns a larger integer if it is not a 
	power of two. The larger integer is the next
	highest power of two.
*/
inline unsigned int glsIsPOW2( unsigned int _value )
{
	unsigned int nPow2 = 1;

	while( _value > nPow2 )
		nPow2 = (nPow2 << 1);

	return nPow2;
}

// more shortcuts
#define	glsGetVectorX( v ) (v[0])
#define glsGetVectorY( v ) (v[1])
#define glsGetVectorZ( v ) (v[2])
#define glsGetVectorW( v ) (v[3])

#define glsSetVectorX( v, x )	((v)[0] = (x))
#define glsSetVectorY( v, y )	((v)[1] = (y))
#define glsSetVectorZ( v, z )	((v)[2] = (z))
#define glsSetVectorW( v, w )	((v)[3] = (w))

inline void glsLoadVector2( Vec2f _v, const float _x, const float _y )
{
	_v[0] = _x;
	_v[1] = _y;
}

inline void glsLoadVector2( Vec2d _v, const float _x, const float _y )
{
	_v[0] = _x;
	_v[1] = _y;
}

inline void glsLoadVector3( Vec3f _v, const float _x, const float _y, const float _z ) 
{
	_v[0] = _x;
	_v[1] = _y;
	_v[2] = _z;
}

inline void glsLoadVector3( Vec3d _v, const double _x, const double _y, const double _z )
{
	_v[0] = _x;
	_v[1] = _y;
	_v[2] = _z;
}

inline void glsLoadVector4( Vec4f _v, const float _x, const float _y, const float _z, const float _w )
{
	_v[0] = _x;
	_v[1] = _y;
	_v[2] = _z;
	_v[3] = _w;
}

inline void glsLoadVector4( Vec4d _v, const double _x, const double _y, const double _z, const double _w )
{
	_v[0] = _x;
	_v[1] = _y;
	_v[2] = _z;
	_v[3] = _w;
}

inline void glsCopyVector2( Vec2f _dst, const Vec2f _src )
{
	memcpy( _dst, _src, sizeof( Vec2f ) );
}

inline void glsCopyVector2( Vec2d _dst, const Vec2d _src )
{
	memcpy( _dst, _src, sizeof( Vec2d ) );
}

inline void glsCopyVector3( Vec3f _dst, const Vec3f _src)
{
	memcpy( _dst, _src, sizeof( Vec3f ) );
}

inline void glsCopyVector3( Vec3d _dst, const Vec3d _src )
{
	memcpy( _dst, _src, sizeof( Vec3d ) );
}

inline void glsCopyVector4( Vec4f _dst, const Vec4f _src )
{
	memcpy( _dst, _src, sizeof( Vec4f ) );
}

inline void glsCopyVector4( Vec4d _dst, const Vec4d _src )
{
	memcpy( _dst, _src, sizeof( Vec4d ) );
}

inline void glsAddVectors2( Vec2f _r, const Vec2f _a, const Vec2f _b )
{
	_r[0] = _a[0] + _b[0];
	_r[1] = _a[1] + _b[1];
}
inline void glsAddVectors2( Vec2d _r, const Vec2d _a, const Vec2d _b )
{
	_r[0] = _a[0] + _b[0];
	_r[1] = _a[1] + _b[1];
}

inline void glsAddVectors3( Vec3f _r, const Vec3f _a, const Vec3f _b )
{
	_r[0] = _a[0] + _b[0];
	_r[1] = _a[1] + _b[1];
	_r[2] = _a[2] + _b[2];
}

inline void glsAddVectors3( Vec3d _r, const Vec3d _a, const Vec3d _b )
{
	_r[0] = _a[0] + _b[0];
	_r[1] = _a[1] + _b[1];
	_r[2] = _a[2] + _b[2];
}

inline void glsAddVectors4( Vec4f _r, const Vec4f _a, const Vec4f _b )
{
	_r[0] = _a[0] + _b[0];
	_r[1] = _a[1] + _b[1];
	_r[2] = _a[2] + _b[2];
	_r[3] = _a[3] + _b[3];
}

inline void glsAddVectors4( Vec4d _r, const Vec4d _a, const Vec4d _b )
{
	_r[0] = _a[0] + _b[0];
	_r[1] = _a[1] + _b[1];
	_r[2] = _a[2] + _b[2];
	_r[3] = _a[3] + _b[3];
}

inline void glsSubtractVectors2( Vec2f _r, const Vec2f _a, const Vec2f _b )
{
	_r[0] = _a[0] - _b[0];
	_r[1] = _a[1] - _b[1]; 
}

inline void glsSubtractVectors2( Vec2d _r, const Vec2d _a, const Vec2d _b )
{
	_r[0] = _a[0] - _b[0];
	_r[1] = _a[1] - _b[1];
}

inline void glsSubtractVectors3( Vec3f _r, const Vec3f _a, const Vec3f _b )
{
	_r[0] = _a[0] - _b[0];
	_r[1] = _a[1] - _b[1];
	_r[2] = _a[2] - _b[2];
}

inline void glsSubtractVectors3( Vec3d _r, const Vec3d _a, const Vec3d _b )
{
	_r[0] = _a[0] - _b[0];
	_r[1] = _a[1] - _b[1];
	_r[2] = _a[2] - _b[2];
}

inline void glsSubtractVectors4( Vec4f _r, const Vec4f _a, const Vec4f _b )
{
	_r[0] = _a[0] - _b[0];
	_r[1] = _a[1] - _b[1];
	_r[2] = _a[2] - _b[2];
	_r[3] = _a[3] - _b[3];
}

inline void glsSubtractVectors4( Vec4d _r, const Vec4d _a, const Vec4d _b )
{
	_r[0] = _a[0] - _b[0];
	_r[1] = _a[1] - _b[1];
	_r[2] = _a[2] - _b[2];
	_r[3] = _a[3] - _b[3];
}

inline void glsScaleVector2( Vec2f _v, const float _scale ) 
{
	_v[0] *= _scale;
	_v[1] *= _scale;
}

inline void glsScaleVector2( Vec2d _v, const double _scale ) 
{
	_v[0] *= _scale;
	_v[1] *= _scale;
}

inline void glsScaleVector3( Vec3f _v, const float _scale ) 
{
	_v[0] *= _scale;
	_v[1] *= _scale;
	_v[2] *= _scale;
}

inline void glsScaleVector3( Vec3d _v, const double _scale ) 
{
	_v[0] *= _scale;
	_v[1] *= _scale;
	_v[2] *= _scale;
}

inline void glsScaleVector4( Vec4f _v, const float _scale ) 
{
	_v[0] *= _scale;
	_v[1] *= _scale;
	_v[2] *= _scale;
	_v[3] *= _scale;
}

inline void glsScaleVector4( Vec4d _v, const double _scale ) 
{
	_v[0] *= _scale;
	_v[1] *= _scale;
	_v[2] *= _scale;
	_v[3] *= _scale;
}

inline void glsCrossProduct3( Vec3f _result, const Vec3f _u, const Vec3f _v )
{
	_result[0] = _u[1] * _v[2] - _v[1] * _u[2];
	_result[1] = -_u[0] * _v[2] + _v[0] * _u[2];
	_result[2] = _u[0] * _v[1] - _v[0] * _u[1];
}

inline void glsCrossProduct3( Vec3d _result, const Vec3d _u, const Vec3d _v )
{
	_result[0] = _u[1] * _v[2] - _v[1] * _u[2];
	_result[1] = -_u[0] * _v[2] + _v[0] * _u[2];
	_result[2] = _u[0] * _v[1] - _v[0] * _u[1];
}

inline float glsDotProduct3( const Vec3f _u, const Vec3f _v )
{
	return _u[0] * _v[0] + _u[1] * _v[1] + _u[2] * _v[2];
}

inline double glsDotProduct3( const Vec3d _u, const Vec3d _v )
{
	return _u[0] * _v[0] + _u[1] * _v[1] + _u[2] * _v[2];
}

inline float glsGetAngleBetweenVectors3( const Vec3f _u, const Vec3f _v )
{
	float dTemp = glsDotProduct3( _u, _v );
	return float( acos( double( dTemp ) ) );	// Double cast just gets rid of compiler warning, no real need
}

inline double glsGetAngleBetweenVectors3( const Vec3d _u, const Vec3d _v )
{
	double dTemp = glsDotProduct3( _u, _v );
	return acos( dTemp );
}

inline float glsGetVectorLengthSquared3( const Vec3f _u )
{
	return (_u[0] * _u[0]) + (_u[1] * _u[1]) + (_u[2] * _u[2]);
}

inline double glsGetVectorLengthSquared3( const Vec3d _u )
{
	return (_u[0] * _u[0]) + (_u[1] * _u[1]) + (_u[2] * _u[2]);
}

inline float glsGetVectorLength3( const Vec3f _u )
{
	return sqrtf( glsGetVectorLengthSquared3( _u ) );
}

inline double glsGetVectorLength3( const Vec3d _u )
{
	return sqrt( glsGetVectorLengthSquared3( _u ) );
}

inline void glsNormalizeVector3( Vec3f _u )
{
	if( glsGetVectorLength3( _u ) != 0.0f )
		glsScaleVector3( _u, 1.0f / glsGetVectorLength3( _u ) );
}

inline void glsNormalizeVector3( Vec3d _u )
{
	if( glsGetVectorLength3( _u ) != 0.0f )
		glsScaleVector3( _u, 1.0 / glsGetVectorLength3( _u ) );
}

float glsGetDistanceSquared3( const Vec3f _u, const Vec3f _v );
double glsGetDistanceSquared3( const Vec3d _u, const Vec3d _v );

inline double glsGetDistance3( const Vec3d _u, const Vec3d _v )
{
	return sqrt( glsGetDistanceSquared3( _u, _v ) );
}

inline float glsGetDistance3( const Vec3f _u, const Vec3f _v )
{
	return sqrtf( glsGetDistanceSquared3( _u, _v ) );
}

inline float glsGetMagnitudeSquared3( const Vec3f _u )
{
	return _u[0] * _u[0] + _u[1] * _u[1] + _u[2] * _u[2];
}

inline double glsGetMagnitudeSquared3(const Vec3d _u)
{
	return _u[0] * _u[0] + _u[1] * _u[1] + _u[2] * _u[2];
}

inline float glsGetMagnitude3(const Vec3f _u)
{
	return sqrtf( glsGetMagnitudeSquared3( _u ) );
}

inline double glsGetMagnitude3(const Vec3d _u)
{
	return sqrt( glsGetMagnitudeSquared3( _u ) );
}

inline void glsCopyMatrix3x3( Matrix3x3f _dst, const Matrix3x3f _src )
{
	memcpy( _dst, _src, sizeof( Matrix3x3f ) );
}

inline void glsCopyMatrix3x3( Matrix3x3d _dst, const Matrix3x3d _src )
{
	memcpy( _dst, _src, sizeof( Matrix3x3d ) ); 
}

inline void glsCopyMatrix4x4( Matrix4x4f _dst, const Matrix4x4f _src )
{
	memcpy( _dst, _src, sizeof( Matrix4x4f ) );
}

inline void glsCopyMatrix4x4( Matrix4x4d _dst, const Matrix4x4d _src )
{
	memcpy( _dst, _src, sizeof( Matrix4x4d ) ); 
}

void glsLoadIdentity3x3( Matrix3x3f _m );
void glsLoadIdentity3x3( Matrix3x3d _m );
void glsLoadIdentity4x4( Matrix4x4f _m );
void glsLoadIdentity4x4( Matrix4x4d _m );

inline void glsGetMatrixColumn3x3( Matrix3x3f _dst, const Matrix3x3f _src, const int _column )
{
	memcpy( _dst, _src + (3 * _column), sizeof( float ) * 3 );
}

inline void glsGetMatrixColumn3x3( Matrix3x3d _dst, const Matrix3x3d _src, const int _column )
{ 
	memcpy( _dst, _src + (3 * _column), sizeof( double ) * 3 );
}

inline void glsSetMatrixColumn3x3( Matrix3x3f _dst, const Matrix3x3f _src, const int _column)
{
	memcpy( _dst + (3 * _column), _src, sizeof( float ) * 3 ); 
}

inline void glsSetMatrixColumn3x3( Matrix3x3d _dst, const Matrix3x3d _src, const int _column )
{
	memcpy( _dst + (3 * _column), _src, sizeof( double ) * 3 );
}

inline void glsGetMatrixColumn4x4( Matrix4x4f _dst, const Matrix4x4f _src, const int _column )
{
	memcpy( _dst, _src + (4 * _column), sizeof( float ) * 4 ); 
}

inline void glsGetMatrixColumn4x4( Matrix4x4d _dst, const Matrix4x4d _src, const int _column )
{
	memcpy( _dst, _src + (4 * _column), sizeof( double ) * 4 ); 
}

inline void glsSetMatrixColumn4x4( Matrix4x4f _dst, const Matrix4x4f _src, const int _column )
{
	memcpy( _dst + (4 * _column), _src, sizeof( float ) * 4 ); 
}

inline void glsSetMatrixColumn4x4( Matrix4x4d _dst, const Matrix4x4d _src, const int _column )
{
	memcpy( _dst + (4 * _column), _src, sizeof( double ) * 4 ); 
}

// Extracts the rotation matrix (3x3) from a 4x4 matrix
inline void glsExtractRotationMatrix3x3( Matrix3x3f _dst, const Matrix4x4f _src )
{	
	memcpy( _dst, _src, sizeof( float ) * 3); // X column
	memcpy( _dst + 3, _src + 4, sizeof( float ) * 3); // Y column
	memcpy( _dst + 6, _src + 8, sizeof( float ) * 3); // Z column
}

// Ditto above, but for doubles
inline void glsExtractRotationMatrix3x3( Matrix3x3d _dst, const Matrix4x4d _src )
{
	memcpy( _dst, _src, sizeof( double ) * 3); // X column
	memcpy( _dst + 3, _src + 4, sizeof( double ) * 3); // Y column
	memcpy( _dst + 6, _src + 8, sizeof( double ) * 3); // Z column
}

// Inject Rotation (3x3) into a full 4x4 matrix...
inline void glsInjectRotationMatrix4x4( Matrix4x4f _dst, const Matrix3x3f _src )
{
	memcpy( _dst, _src, sizeof( float ) * 4);
	memcpy( _dst + 4, _src + 4, sizeof( float ) * 4);
	memcpy( _dst + 8, _src + 8, sizeof( float ) * 4);
}

// Ditto above for doubles
inline void glsInjectRotationMatrix4x4( Matrix4x4d _dst, const Matrix3x3d _src )
{
	memcpy( _dst, _src, sizeof( double ) * 4);
	memcpy( _dst + 4, _src + 4, sizeof( double ) * 4);
	memcpy( _dst + 8, _src + 8, sizeof( double ) * 4);
}

void glsVectorMultiply4( Vec4f _product, const Vec4f _a, const Vec4f _b );
void glsVectorMultiply4( Vec4d _product, const Vec4d _a, const Vec4d _b );
void glsVectorMultiply3( Vec3f _product, const Vec3f _a, const Vec3f _b );
void glsVectorMultiply3( Vec3d _product, const Vec3d _a, const Vec3d _b );
void glsVectorMultiply2( Vec2f _product, const Vec2f _a, const Vec2f _b );
void glsVectorMultiply2( Vec2d _product, const Vec2d _a, const Vec2d _b );

void glsMatrixMultiply4x4( Matrix4x4f _product, const Matrix4x4f _a, const Matrix4x4f _b );
void glsMatrixMultiply4x4( Matrix4x4d _product, const Matrix4x4d _a, const Matrix4x4d _b );
void glsMatrixMultiply3x3( Matrix3x3f _product, const Matrix3x3f _a, const Matrix3x3f _b );
void glsMatrixMultiply3x3( Matrix3x3d _product, const Matrix3x3d _a, const Matrix3x3d _b );

// Just transform a 3 compoment vector
__inline void glsTransformVector3( Vec3f _vOut, const Vec3f _v, const Matrix4x4f _m )
{
	_vOut[0] = _m[0] * _v[0] + _m[4] * _v[1] + _m[8] *  _v[2] + _m[12];// * _v[3];	// Assuming 1 
	_vOut[1] = _m[1] * _v[0] + _m[5] * _v[1] + _m[9] *  _v[2] + _m[13];// * _v[3];	
	_vOut[2] = _m[2] * _v[0] + _m[6] * _v[1] + _m[10] * _v[2] + _m[14];// * _v[3];	
	//_vOut[3] = _m[3] * _v[0] + _m[7] * _v[1] + _m[11] * _v[2] + _m[15] * _v[3];
}

// Ditto above, but for doubles
__inline void glsTransformVector3( Vec3d _vOut, const Vec3d _v, const Matrix4x4d _m )
{
	_vOut[0] = _m[0] * _v[0] + _m[4] * _v[1] + _m[8] *  _v[2] + _m[12];// * _v[3];	 
	_vOut[1] = _m[1] * _v[0] + _m[5] * _v[1] + _m[9] *  _v[2] + _m[13];// * _v[3];	
	_vOut[2] = _m[2] * _v[0] + _m[6] * _v[1] + _m[10] * _v[2] + _m[14];// * _v[3];	
	//_vOut[3] = _m[3] * _v[0] + _m[7] * _v[1] + _m[11] * _v[2] + _m[15] * _v[3];
}

// Full four component transform
__inline void glsTransformVector4( Vec4f _vOut, const Vec4f _v, const Matrix4x4f _m )
{
	_vOut[0] = _m[0] * _v[0] + _m[4] * _v[1] + _m[8] *  _v[2] + _m[12] * _v[3];	 
	_vOut[1] = _m[1] * _v[0] + _m[5] * _v[1] + _m[9] *  _v[2] + _m[13] * _v[3];	
	_vOut[2] = _m[2] * _v[0] + _m[6] * _v[1] + _m[10] * _v[2] + _m[14] * _v[3];	
	_vOut[3] = _m[3] * _v[0] + _m[7] * _v[1] + _m[11] * _v[2] + _m[15] * _v[3];
}

// Ditto above, but for doubles
__inline void glsTransformVector4( Vec4d _vOut, const Vec4d _v, const Matrix4x4d _m )
{
	_vOut[0] = _m[0] * _v[0] + _m[4] * _v[1] + _m[8] *  _v[2] + _m[12] * _v[3];	 
	_vOut[1] = _m[1] * _v[0] + _m[5] * _v[1] + _m[9] *  _v[2] + _m[13] * _v[3];	
	_vOut[2] = _m[2] * _v[0] + _m[6] * _v[1] + _m[10] * _v[2] + _m[14] * _v[3];	
	_vOut[3] = _m[3] * _v[0] + _m[7] * _v[1] + _m[11] * _v[2] + _m[15] * _v[3];
}

// Just do the rotation, not the translation... this is usually done with a 3x3
// Matrix.
__inline void glsRotateVector( Vec3f _vOut, const Vec3f _p, const Matrix3x3f _m )
{
	_vOut[0] = _m[0] * _p[0] + _m[3] * _p[1] + _m[6] * _p[2];	
	_vOut[1] = _m[1] * _p[0] + _m[4] * _p[1] + _m[7] * _p[2];	
	_vOut[2] = _m[2] * _p[0] + _m[5] * _p[1] + _m[8] * _p[2];	
}

// Ditto above, but for doubles
__inline void glsRotateVector( Vec3d _vOut, const Vec3d _p, const Matrix3x3d _m )
{
	_vOut[0] = _m[0] * _p[0] + _m[3] * _p[1] + _m[6] * _p[2];	
	_vOut[1] = _m[1] * _p[0] + _m[4] * _p[1] + _m[7] * _p[2];	
	_vOut[2] = _m[2] * _p[0] + _m[5] * _p[1] + _m[8] * _p[2];	
}

inline void glsScaleMatrix3x3( Matrix3x3f _m, float _xScale, float _yScale, float _zScale )
{ 
	glsLoadIdentity3x3( _m );
	_m[0] = _xScale;
	_m[4] = _yScale;
	_m[8] = _zScale; 
}

inline void glsScaleMatrix3x3( Matrix3x3f _m, const Vec3f _vScale )
{
	glsLoadIdentity3x3( _m );
	_m[0] = _vScale[0];
	_m[4] = _vScale[1];
	_m[8] = _vScale[2];
}

inline void glsScaleMatrix3x3( Matrix3x3d _m, double _xScale, double _yScale, double _zScale )
{ 
	glsLoadIdentity3x3( _m );
	_m[0] = _xScale;
	_m[4] = _yScale;
	_m[8] = _zScale; 
}

inline void glsScaleMatrix3x3( Matrix3x3d _m, const Vec3d _vScale )
{
	glsLoadIdentity3x3( _m );
	_m[0] = _vScale[0];
	_m[4] = _vScale[1];
	_m[8] = _vScale[2]; 
}

inline void glsScaleMatrix4x4( Matrix4x4f _m, float _xScale, float _yScale, float _zScale )
{ 
	glsLoadIdentity4x4( _m );
	_m[0] = _xScale;
	_m[5] = _yScale;
	_m[10] = _zScale; 
}

inline void glsScaleMatrix4x4( Matrix4x4f _m, const Vec3f _vScale )
{ 
	glsLoadIdentity4x4( _m );
	_m[0] = _vScale[0];
	_m[5] = _vScale[1];
	_m[10] = _vScale[2];
}

inline void glsScaleMatrix4x4( Matrix4x4d _m, double _xScale, double _yScale, double _zScale )
{ 
	glsLoadIdentity4x4( _m );
	_m[0] = _xScale;
	_m[5] = _yScale;
	_m[10] = _zScale; 
}

inline void glsScaleMatrix4x4( Matrix4x4d _m, const Vec3d _vScale )
{ 
	glsLoadIdentity4x4( _m );
	_m[0] = _vScale[0];
	_m[5] = _vScale[1];
	_m[10] = _vScale[2]; 
}

void glsMakePerspectiveMatrix( Matrix4x4f _mProjection, float _fFov, float _fAspect, float _zMin, float _zMax );
void glsMakeOrthographicMatrix( Matrix4x4f _mProjection, float _xMin, float _xMax, float _yMin, float _yMax, float _zMin, float _zMax );

void glsRotationMatrix3x3( Matrix3x3f _m, float _angle, float _x, float _y, float _z);
void glsRotationMatrix3x3( Matrix3x3d _m, double _angle, double _x, double _y, double _z);
void glsRotationMatrix4x4( Matrix4x4f _m, float _angle, float _x, float _y, float _z);
void glsRotationMatrix4x4( Matrix4x4d _m, double _angle, double _x, double _y, double _z);

inline void glsTranslationMatrix4x4( Matrix4x4f _m, float _x, float _y, float _z )
{
	glsLoadIdentity4x4( _m );
	_m[12] = _x;
	_m[13] = _y;
	_m[14] = _z; 
}

inline void glsTranslationMatrix4x4( Matrix4x4d _m, double _x, double _y, double _z )
{ 
	glsLoadIdentity4x4( _m );
	_m[12] = _x;
	_m[13] = _y;
	_m[14] = _z;
}

void glsInvertMatrix4x4( Matrix4x4f _mInverse, const Matrix4x4f _m );
void glsInvertMatrix4x4( Matrix4x4d _mInverse, const Matrix4x4d _m );
void glsTransposeMatrix4x4( Matrix4x4f _mTranspose, const Matrix4x4f _m );
void glsTransposeMatrix4x4( Matrix4x4d _mTranspose, const Matrix4x4d _m );


// Find a normal from three points
void glsFindNormal( Vec3f _result, const Vec3f _point1, const Vec3f _point2, const Vec3f _point3 );
void glsFindNormal( Vec3d _result, const Vec3d _point1, const Vec3d _point2, const Vec3d _point3 );

// Calculates the signed distance of a point to a plane
inline float glsGetDistanceToPlane( const Vec3f _point, const Vec4f _plane )
{
	return _point[0] * _plane[0] + _point[1] * _plane[1] + _point[2] * _plane[2] + _plane[3]; 
}

inline double glsGetDistanceToPlane( const Vec3d _point, const Vec4d _plane )
{
	return _point[0] * _plane[0] + _point[1] * _plane[1] + _point[2] * _plane[2] + _plane[3];
}

// Get plane equation from three points
void glsGetPlaneEquation( Vec4f _planeEq, const Vec3f _p1, const Vec3f _p2, const Vec3f _p3 );
void glsGetPlaneEquation( Vec4d _planeEq, const Vec3d _p1, const Vec3d _p2, const Vec3d _p3 );

// Determine if a ray intersects a sphere
// Return value is < 0 if the ray does not intersect
// Return value is 0.0 if ray is tangent
// Positive value is distance to the intersection point
double glsRaySphereTest( const Vec3d _point, const Vec3d _ray, const Vec3d _sphereCenter, double _sphereRadius );
float glsRaySphereTest( const Vec3f _point, const Vec3f _ray, const Vec3f _sphereCenter, float _sphereRadius);

void glsProjectXY( Vec2f _vPointOut, const Matrix4x4f _mModelView, const Matrix4x4f _mProjection, const int _iViewPort[4], const Vec3f _vPointIn );    
void glsProjectXYZ( Vec3f _vPointOut, const Matrix4x4f _mModelView, const Matrix4x4f _mProjection, const int _iViewPort[4], const Vec3f _vPointIn );

void glsCatmullRom( Vec3f _vOut, const Vec3f _vP0, const Vec3f _vP1, const Vec3f _vP2, const Vec3f _vP3, float _t );
void glsCatmullRom( Vec3d _vOut, const Vec3d _vP0, const Vec3d _vP1, const Vec3d _vP2, const Vec3d _vP3, double _t );

inline bool glsCloseEnough( const float _fCandidate, const float _fCompare, const float _fEpsilon)
{
	return (fabs( _fCandidate - _fCompare ) < _fEpsilon);
}

inline bool glsCloseEnough( const double _dCandidate, const double _dCompare, const double _dEpsilon)
{
	return (fabs( _dCandidate - _dCompare ) < _dEpsilon);
}    

void glsCalculateTangentBasis( Vec3f _vTangent, const Vec3f _pvTriangle[3], const Vec2f _pvTexCoords[3], const Vec3f _N);

double glsSmoothStep( const double _edge1, const double _edge2, const double _x);
float glsSmoothStep( const float _edge1, const float _edge2, const float _x);

void glsMakePlanarShadowMatrix( Matrix4x4d _proj, const Vec4d _planeEq, const Vec3d _vLightPos );
void glsMakePlanarShadowMatrix( Matrix4x4f _proj, const Vec4f _planeEq, const Vec3f _vLightPos );

double glsClosestPointOnRay( Vec3d _vPointOnRay, const Vec3d _vRayOrigin, const Vec3d _vUnitRayDir, const Vec3d _vPointInSpace);

float glsClosestPointOnRay( Vec3f vPointOnRay, const Vec3f _vRayOrigin, const Vec3f _vUnitRayDir, const Vec3f _vPointInSpace);

void glsGetVec3FromMat4( Vec3f _VecToFillOut, Matrix4x4f _Mat, int _WhichVec);

void glsLookAt(Matrix4x4f _Source, Matrix4x4f _Destination);

inline void glsBiasMatrixInverse( Matrix4x4f _BI )
{
	_BI[0] = 2.0f; _BI[4] = 0.0f; _BI[8] = 0.0f; _BI[12] = -1.0f;
	_BI[1] = 0.0f; _BI[5] = 2.0f; _BI[9] = 0.0f; _BI[13] = -1.0f;
	_BI[2] = 0.0f; _BI[6] = 0.0f; _BI[10] = 2.0f; _BI[14] = -1.0f;
	_BI[3] = 0.0f; _BI[7] = 0.0f; _BI[11] = 0.0f; _BI[15] = 1.0f;
}

inline void glsBiasMatrixInverse( Matrix4x4d _BI )
{
	_BI[0] = 2.0; _BI[4] = 0.0; _BI[8] = 0.0; _BI[12] = -1.0;
	_BI[1] = 0.0; _BI[5] = 2.0; _BI[9] = 0.0; _BI[13] = -1.0;
	_BI[2] = 0.0; _BI[6] = 0.0; _BI[10] = 2.0; _BI[14] = -1.0;
	_BI[3] = 0.0; _BI[7] = 0.0; _BI[11] = 0.0; _BI[15] = 1.0;
}