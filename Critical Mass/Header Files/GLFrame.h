/*
File:		GLFrame.h
Author:		Bob Pasekoff
Updated:	7/16/2013

*/

#pragma once

#include "glsMath.h"

class GLFrame
{
protected:
	Vec3f origin;
	Vec3f forward;
	Vec3f up;

public:
	// Default position and orientation. At the origin, looking
	// down the positive Z axis (right handed coordinate system).
	GLFrame( void )
	{
		// At origin
		origin[0] = 0.0f; origin[1] = 0.0f; origin[2] = 0.0f; 

		// Up is up (+Y)
		up[0] = 0.0f; up[1] = 1.0f; up[2] = 0.0f;

		// Forward is -Z (default OpenGL)
		forward[0] = 0.0f; forward[1] = 0.0f; forward[2] = -1.0f;
	}

	// Set Location
	inline void SetOrigin( const Vec3f _point )
	{
		glsCopyVector3( origin, _point );
	}

	inline void SetOrigin( float _x, float _y, float _z ) 
	{ 
		origin[0] = _x; origin[1] = _y; origin[2] = _z;
	}

	inline void GetOrigin( Vec3f _point )
	{
		glsCopyVector3( _point, origin ); 
	}

	inline float GetOriginX( void ) { return origin[0]; }
	inline float GetOriginY( void ) { return origin[1]; } 
	inline float GetOriginZ( void ) { return origin[2]; }

	// Set Forward Direction
	inline void SetForwardVector( const Vec3f _direction )
	{
		glsCopyVector3( forward, _direction );
	}

	inline void SetForwardVector( float _x, float _y, float _z )
	{
		forward[0] = _x;
		forward[1] = _y;
		forward[2] = _z;
	}

	inline void GetForwardVector( Vec3f _vector )
	{
		glsCopyVector3( _vector, forward );
	}

	// Set Up Direction
	inline void SetUpVector( const Vec3f _direction )
	{
		glsCopyVector3( up, _direction );
	}

	inline void SetUpVector( float _x, float _y, float _z )
	{
		up[0] = _x;
		up[1] = _y;
		up[2] = _z;
	}

	inline void GetUpVector( Vec3f _vector )
	{
		glsCopyVector3( _vector, up );
	}

	// Get Axes
	inline void GetZAxis( Vec3f _vector ) { GetForwardVector( _vector ); }
	inline void GetYAxis( Vec3f _vector ) { GetUpVector( _vector ); }
	inline void GetXAxis( Vec3f _vector ) { glsCrossProduct3( _vector, up, forward ); }

	// Translate along orthonormal axis... world or local
	inline void TranslateWorld( float _x, float _y, float _z )
	{
		origin[0] += _x;
		origin[1] += _y;
		origin[2] += _z;
	}

	inline void TranslateLocal( float _x, float _y, float _z )
	{
		MoveForward( _z );
		MoveUp( _y );
		MoveRight( _x );
	}

	// Move Forward (along Z axis)
	inline void MoveForward( float _delta )
	{
		// Move along direction of front direction
		origin[0] += forward[0] * _delta;
		origin[1] += forward[1] * _delta;
		origin[2] += forward[2] * _delta;
	}

	// Move along Y axis
	inline void MoveUp( float _delta )
	{
		// Move along direction of up direction
		origin[0] += up[0] * _delta;
		origin[1] += up[1] * _delta;
		origin[2] += up[2] * _delta;
	}

	// Move along X axis
	inline void MoveRight( float _delta )
	{
		// Move along direction of right vector
		Vec3f vCross;
		glsCrossProduct3(vCross, up, forward);

		origin[0] += vCross[0] * _delta;
		origin[1] += vCross[1] * _delta;
		origin[2] += vCross[2] * _delta;
	}

	// Just assemble the matrix
	void GetMatrix( Matrix4x4f _matrix, bool _rotationOnly = false )
	{
		// Calculate the right side (x) vector, drop it right into the matrix
		Vec3f vXAxis;
		glsCrossProduct3( vXAxis, up, forward );

		// Set matrix column does not fill in the fourth value...
		glsSetMatrixColumn4x4( _matrix, vXAxis, 0 );
		_matrix[3] = 0.0f;

		// Y Column
		glsSetMatrixColumn4x4( _matrix, up, 1 );
		_matrix[7] = 0.0f;       

		// Z Column
		glsSetMatrixColumn4x4( _matrix, forward, 2 );
		_matrix[11] = 0.0f;

		// Translation (already done)
		if( _rotationOnly == true )
		{
			_matrix[12] = 0.0f;
			_matrix[13] = 0.0f;
			_matrix[14] = 0.0f;
		}
		else
			glsSetMatrixColumn4x4( _matrix, origin, 3 );

		_matrix[15] = 1.0f;
	}

	// Assemble the camera matrix
	void GetCameraMatrix( Matrix4x4f _m, bool _rotationOnly = false )
	{
		Vec3f x, z;

		// Make rotation matrix
		// Z vector is reversed
		z[0] = -forward[0];
		z[1] = -forward[1];
		z[2] = -forward[2];

		// X vector = Y cross Z 
		glsCrossProduct3( x, up, z );

		// Matrix has no translation information and is
		// transposed.... (rows instead of columns)
#define M(row,col)  _m[col*4+row]
		M(0, 0) = x[0];
		M(0, 1) = x[1];
		M(0, 2) = x[2];
		M(0, 3) = 0.0;
		M(1, 0) = up[0];
		M(1, 1) = up[1];
		M(1, 2) = up[2];
		M(1, 3) = 0.0;
		M(2, 0) = z[0];
		M(2, 1) = z[1];
		M(2, 2) = z[2];
		M(2, 3) = 0.0;
		M(3, 0) = 0.0;
		M(3, 1) = 0.0;
		M(3, 2) = 0.0;
		M(3, 3) = 1.0;
#undef M


		if( _rotationOnly )
			return;

		// Apply translation too
		Matrix4x4f trans, M;
		glsTranslationMatrix4x4( trans, -origin[0], -origin[1], -origin[2] );  

		glsMatrixMultiply4x4( M, _m, trans );

		// Copy result back into m
		memcpy( _m, M, sizeof(float)*16 );
	}


	// Rotate around local Y
	void RotateLocalY( float _angle )
	{
		Matrix4x4f rotMat;

		// Just Rotate around the up vector
		// Create a rotation matrix around my Up (Y) vector
		glsRotationMatrix4x4( rotMat, _angle, up[0], up[1], up[2] );

		Vec3f newVect;

		// Rotate forward pointing vector (inlined 3x3 transform)
		newVect[0] = rotMat[0] * forward[0] + rotMat[4] * forward[1] + rotMat[8] *  forward[2];	
		newVect[1] = rotMat[1] * forward[0] + rotMat[5] * forward[1] + rotMat[9] *  forward[2];	
		newVect[2] = rotMat[2] * forward[0] + rotMat[6] * forward[1] + rotMat[10] * forward[2];	
		glsCopyVector3( forward, newVect );
	}


	// Rotate around local Z
	void RotateLocalZ( float _angle )
	{
		Matrix4x4f rotMat;

		// Only the up vector needs to be rotated
		glsRotationMatrix4x4( rotMat, _angle, forward[0], forward[1], forward[2] );

		Vec3f newVect;
		newVect[0] = rotMat[0] * up[0] + rotMat[4] * up[1] + rotMat[8] *  up[2];	
		newVect[1] = rotMat[1] * up[0] + rotMat[5] * up[1] + rotMat[9] *  up[2];	
		newVect[2] = rotMat[2] * up[0] + rotMat[6] * up[1] + rotMat[10] * up[2];	
		glsCopyVector3( up, newVect );
	}

	void RotateLocalX( float _angle )
	{
		Matrix3x3f rotMat;
		Vec3f localX;
		Vec3f rotVec;

		// Get the local X axis
		glsCrossProduct3( localX, up, forward );

		// Make a Rotation Matrix
		glsRotationMatrix3x3( rotMat, _angle, localX[0], localX[1], localX[2] );

		// Rotate Y, and Z
		glsRotateVector( rotVec, up, rotMat );
		glsCopyVector3( up, rotVec );

		glsRotateVector( rotVec, forward, rotMat );
		glsCopyVector3( forward, rotVec );
	}


	// Reset axes to make sure they are orthonormal. This should be called on occasion
	// if the matrix is long-lived and frequently transformed.
	void Normalize( void )
	{
		Vec3f vCross;

		// Calculate cross product of up and forward vectors
		glsCrossProduct3( vCross, up, forward );

		// Use result to recalculate forward vector
		glsCrossProduct3( forward, vCross, up );	

		// Also check for unit length...
		glsNormalizeVector3( up );
		glsNormalizeVector3( forward );
	}


	// Rotate in world coordinates...
	void RotateWorld( float _angle, float _x, float _y, float _z )
	{
		Matrix4x4f rotMat;

		// Create the Rotation matrix
		glsRotationMatrix4x4( rotMat, _angle, _x, _y, _z );

		Vec3f newVect;

		// Transform the up axis (inlined 3x3 rotation)
		newVect[0] = rotMat[0] * up[0] + rotMat[4] * up[1] + rotMat[8] *  up[2];	
		newVect[1] = rotMat[1] * up[0] + rotMat[5] * up[1] + rotMat[9] *  up[2];	
		newVect[2] = rotMat[2] * up[0] + rotMat[6] * up[1] + rotMat[10] * up[2];	
		glsCopyVector3(up, newVect);

		// Transform the forward axis
		newVect[0] = rotMat[0] * forward[0] + rotMat[4] * forward[1] + rotMat[8] *  forward[2];	
		newVect[1] = rotMat[1] * forward[0] + rotMat[5] * forward[1] + rotMat[9] *  forward[2];	
		newVect[2] = rotMat[2] * forward[0] + rotMat[6] * forward[1] + rotMat[10] * forward[2];	
		glsCopyVector3(forward, newVect);
	}


	// Rotate around a local axis
	void RotateLocal( float _angle, float _x, float _y, float _z ) 
	{
		Vec3f vWorldVect;
		Vec3f vLocalVect;
		glsLoadVector3( vLocalVect, _x, _y, _z );

		LocalToWorld( vLocalVect, vWorldVect, true );
		RotateWorld( _angle, vWorldVect[0], vWorldVect[1], vWorldVect[2] );
	}


	// Convert Coordinate Systems
	// This is pretty much, do the transformation represented by the rotation
	// and position on the point
	// Is it better to stick to the convention that the destination always comes
	// first, or use the conventions that "sounds" like the function...
	void LocalToWorld( const Vec3f _local, Vec3f _world, bool _rotOnly = false )
	{
		// Create the rotation matrix based on the vectors
		Matrix4x4f rotMat;

		GetMatrix( rotMat, true );

		// Do the rotation (inline it, and remove 4th column...)
		_world[0] = rotMat[0] * _local[0] + rotMat[4] * _local[1] + rotMat[8] *  _local[2];	
		_world[1] = rotMat[1] * _local[0] + rotMat[5] * _local[1] + rotMat[9] *  _local[2];	
		_world[2] = rotMat[2] * _local[0] + rotMat[6] * _local[1] + rotMat[10] * _local[2];	

		// Translate the point
		if( !_rotOnly )
		{
			_world[0] += origin[0];
			_world[1] += origin[1];
			_world[2] += origin[2];
		}
	}

	// Change world coordinates into "local" coordinates
	void WorldToLocal( const Vec3f _world, Vec3f _local )
	{
		// Translate the origin
		Vec3f vNewWorld;
		vNewWorld[0] = _world[0] - origin[0];
		vNewWorld[1] = _world[1] - origin[1];
		vNewWorld[2] = _world[2] - origin[2];

		// Create the rotation matrix based on the vectors
		Matrix4x4f rotMat;
		Matrix4x4f invMat;
		GetMatrix( rotMat, true );

		// Do the rotation based on inverted matrix
		glsInvertMatrix4x4( invMat, rotMat );

		_local[0] = invMat[0] * vNewWorld[0] + invMat[4] * vNewWorld[1] + invMat[8] *  vNewWorld[2];	
		_local[1] = invMat[1] * vNewWorld[0] + invMat[5] * vNewWorld[1] + invMat[9] *  vNewWorld[2];	
		_local[2] = invMat[2] * vNewWorld[0] + invMat[6] * vNewWorld[1] + invMat[10] * vNewWorld[2];	
	}

	// Transform a point by frame matrix
	void TransformPoint( Vec3f _pointSrc, Vec3f _pointDst )
	{
		Matrix4x4f m;
		GetMatrix( m, false );    // Rotate and translate
		_pointSrc[0] = m[0] * _pointSrc[0] + m[4] * _pointSrc[1] + m[8] *  _pointSrc[2] + m[12];// * v[3];	 
		_pointSrc[1] = m[1] * _pointSrc[0] + m[5] * _pointSrc[1] + m[9] *  _pointSrc[2] + m[13];// * v[3];	
		_pointSrc[2] = m[2] * _pointSrc[0] + m[6] * _pointSrc[1] + m[10] * _pointSrc[2] + m[14];// * v[3];	
	}

	// Rotate a vector by frame matrix
	void RotateVector( Vec3f _vectorSrc, Vec3f _vectorDst )
	{
		Matrix4x4f m;
		GetMatrix( m, true );    // Rotate only

		_vectorDst[0] = m[0] * _vectorSrc[0] + m[4] * _vectorSrc[1] + m[8] *  _vectorSrc[2];	 
		_vectorDst[1] = m[1] * _vectorSrc[0] + m[5] * _vectorSrc[1] + m[9] *  _vectorSrc[2];	
		_vectorDst[2] = m[2] * _vectorSrc[0] + m[6] * _vectorSrc[1] + m[10] * _vectorSrc[2];	
	}
};