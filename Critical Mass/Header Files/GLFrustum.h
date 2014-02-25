/*
	File:		GLFrustum.h
	Author:		Bob Pasekoff
	Updated:	7/16/2013

*/

#include "../Header Files/stdafx.h"
#include "../Header Files/glsMath.h"
#include "../Header Files/GLFrame.h"

class GLFrustum
{
public:
	GLFrustum( void )
	{
		SetOrthographic( -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f );
	}

	// Set the View Frustum
	GLFrustum( GLfloat _fov, GLfloat _aspect, GLfloat _near, GLfloat _far )
	{
		SetPerspective( _fov, _aspect, _near, _far );
	}

	GLFrustum( GLfloat _xMin, GLfloat _xMax, GLfloat _yMin, GLfloat _yMax, GLfloat _zMin, GLfloat _zMax )
	{
		SetOrthographic( _xMin, _xMax, _yMin, _yMax, _zMin, _zMax );
	}

	const Matrix4x4f& GetProjectionMatrix( void )
	{
		return projMatrix;
	}
  
	void SetOrthographic( GLfloat _xMin, GLfloat _xMax, GLfloat _yMin, GLfloat _yMax, GLfloat _zMin, GLfloat _zMax )
	{
		glsMakeOrthographicMatrix( projMatrix, _xMin, _xMax, _yMin, _yMax, _zMin, _zMax );
		projMatrix[15] = 1.0f;

		// Near Upper Left
		nearUL[0] = _xMin;
		nearUL[1] = _yMax;
		nearUL[2] = _zMin;
		nearUL[3] = 1.0f;

		// Near Lower Left
		nearLL[0] = _xMin;
		nearLL[1] = _yMin;
		nearLL[2] = _zMin;
		nearLL[3] = 1.0f;

		// Near Upper Right
		nearUR[0] = _xMax;
		nearUR[1] = _yMax;
		nearUR[2] = _zMin;
		nearUR[3] = 1.0f;

		// Near Lower Right
		nearLR[0] = _xMax;
		nearLR[1] = _yMin;
		nearLR[2] = _zMin;
		nearLR[3] = 1.0f;

		// Far Upper Left
		farUL[0] = _xMin;
		farUL[1] = _yMax;
		farUL[2] = _zMax;
		farUL[3] = 1.0f;

		// Far Lower Left
		farLL[0] = _xMin;
		farLL[1] = _yMin;
		farLL[2] = _zMax;
		farLL[3] = 1.0f;

		// Far Upper Right
		farUR[0] = _xMax;
		farUR[1] = _yMax;
		farUR[2] = _zMax;
		farUR[3] = 1.0f;

		// Far Lower Right
		farLR[0] = _xMax;
		farLR[1] = _yMin;
		farLR[2] = _zMax;
		farLR[3] = 1.0f;
	}

	// Perspective Matrix Projection
	void SetPerspective( float _fov, float _aspect, float _near, float _far )
	{
		float xmin, xmax, ymin, ymax;       // Dimensions of near clipping plane
		float xFmin, xFmax, yFmin, yFmax;   // Dimensions of far clipping plane

		// Do the Math for the near clipping plane
		ymax = _near * float(tan( _fov * GLS_PI / 360.0 ));
		ymin = -ymax;
		xmin = ymin * _aspect;
		xmax = -xmin;

		// Construct the projection matrix
		glsLoadIdentity4x4( projMatrix );
		projMatrix[0] = (2.0f * _near)/(xmax - xmin);
		projMatrix[5] = (2.0f * _near)/(ymax - ymin);
		projMatrix[8] = (xmax + xmin) / (xmax - xmin);
		projMatrix[9] = (ymax + ymin) / (ymax - ymin);
		projMatrix[10] = -((_far + _near)/(_far - _near));
		projMatrix[11] = -1.0f;
		projMatrix[14] = -((2.0f * _far * _near)/(_far - _near));
		projMatrix[15] = 0.0f;

		// Do the Math for the far clipping plane
		yFmax = _far * float(tan( _fov * GLS_PI / 360.0 ));
		yFmin = -yFmax;
		xFmin = yFmin * _aspect;
		xFmax = -xFmin;


		// Fill in values for untransformed Frustum corners
		// Near Upper Left
		nearUL[0] = xmin;
		nearUL[1] = ymax;
		nearUL[2] = -_near;
		nearUL[3] = 1.0f;

		// Near Lower Left
		nearLL[0] = xmin;
		nearLL[1] = ymin;
		nearLL[2] = -_near;
		nearLL[3] = 1.0f;

		// Near Upper Right
		nearUR[0] = xmax;
		nearUR[1] = ymax;
		nearUR[2] = -_near;
		nearUR[3] = 1.0f;

		// Near Lower Right
		nearLR[0] = xmax;
		nearLR[1] = ymin;
		nearLR[2] = -_near;
		nearLR[3] = 1.0f;

		// Far Upper Left
		farUL[0] = xFmin;
		farUL[1] = yFmax;
		farUL[2] = -_far;
		farUL[3] = 1.0f;

		// Far Lower Left
		farLL[0] = xFmin;
		farLL[1] = yFmin;
		farLL[2] = -_far;
		farLL[3] = 1.0f;

		// Far Upper Right
		farUR[0] = xFmax;
		farUR[1] = yFmax;
		farUR[2] = -_far;
		farUR[3] = 1.0f;

		// Far Lower Right
		farLR[0] = xFmax;
		farLR[1] = yFmin;
		farLR[2] = -_far;
		farLR[3] = 1.0f;
	}


	void Transform( GLFrame& _camera )
	{
		// Workspace
		Matrix4x4f rotMat;
		Vec3f vForward, vUp, vCross;
		Vec3f vOrigin;

		_camera.GetForwardVector(vForward);
		vForward[0] = -vForward[0];
		vForward[1] = -vForward[1];
		vForward[2] = -vForward[2];

		_camera.GetUpVector( vUp );
		_camera.GetOrigin( vOrigin );

		// Calculate the right side (x) vector
		glsCrossProduct3( vCross, vUp, vForward );

		// The Matrix
		// X Column
		memcpy( rotMat, vCross, sizeof( float ) * 3 );
		rotMat[3] = 0.0f;

		// Y Column
		memcpy( &rotMat[4], vUp, sizeof( float ) * 3 );
		rotMat[7] = 0.0f;       

		// Z Column
		memcpy( &rotMat[8], vForward, sizeof( float ) * 3 );
		rotMat[11] = 0.0f;

		// Translation
		rotMat[12] = vOrigin[0];
		rotMat[13] = vOrigin[1];
		rotMat[14] = vOrigin[2];
		rotMat[15] = 1.0f;

		// Transform the frustum corners
		glsTransformVector4( nearULT, nearUL, rotMat );
		glsTransformVector4( nearLLT, nearLL, rotMat );
		glsTransformVector4( nearURT, nearUR, rotMat );
		glsTransformVector4( nearLRT, nearLR, rotMat );
		glsTransformVector4( farULT, farUL, rotMat );
		glsTransformVector4( farLLT, farLL, rotMat );
		glsTransformVector4( farURT, farUR, rotMat );
		glsTransformVector4( farLRT, farLR, rotMat );

		// Near and Far Planes
		glsGetPlaneEquation( nearPlane, nearULT, nearLLT, nearLRT );
		glsGetPlaneEquation( farPlane, farULT, farURT, farLRT );

		// Top and Bottom Planes
		glsGetPlaneEquation( topPlane, nearULT, nearURT, farURT );
		glsGetPlaneEquation( bottomPlane, nearLLT, farLLT, farLRT );

		// Left and right planes
		glsGetPlaneEquation( leftPlane, nearLLT, nearULT, farULT );
		glsGetPlaneEquation( rightPlane, nearLRT, farLRT, farURT );
	}

	// Allow expanded version of sphere test
	bool TestSphere( float _x, float _y, float _z, float _radius )
	{
		Vec3f vPoint;
		vPoint[0] = _x;
		vPoint[1] = _y;
		vPoint[2] = _z;

		return TestSphere( vPoint, _radius );
	}


	bool TestSphere( Vec3f _point, float _radius )
	{
		float fDist;

		// Near Plane - See if it is behind
		fDist = glsGetDistanceToPlane( _point, nearPlane );
		if( fDist + _radius <= 0.0 )
			return false;

		// Distance to far plane
		fDist = glsGetDistanceToPlane( _point, farPlane );
		if( fDist + _radius <= 0.0 )
			return false;

		fDist = glsGetDistanceToPlane( _point, leftPlane );
		if( fDist + _radius <= 0.0 )
			return false;

		fDist = glsGetDistanceToPlane( _point, rightPlane );
		if( fDist + _radius <= 0.0 )
			return false;

		fDist = glsGetDistanceToPlane( _point, bottomPlane );
		if( fDist + _radius <= 0.0 )
			return false;

		fDist = glsGetDistanceToPlane( _point, topPlane );
		if( fDist + _radius <= 0.0 )
			return false;

		return true;
	}

protected:
	// The projection matrix for this frustum
	Matrix4x4f projMatrix;	

	// Untransformed corners of the frustum
	Vec4f  nearUL, nearLL, nearUR, nearLR;
	Vec4f  farUL,  farLL,  farUR,  farLR;

	// Transformed corners of Frustum
	Vec4f  nearULT, nearLLT, nearURT, nearLRT;
	Vec4f  farULT,  farLLT,  farURT,  farLRT;

	// Base and Transformed plane equations
	Vec4f nearPlane, farPlane, leftPlane, rightPlane;
	Vec4f topPlane, bottomPlane;
};