/*
File:		GLGeometryTransform.h
Author:		Bob Pasekoff
Updated:	7/16/2013

*/

#include "../Header Files/stdafx.h"
#include "../Header Files/GLSetup.h"

class GLGeometryTransform
{
public:
	GLGeometryTransform( void ) {}

	inline void SetModelViewMatrixStack( GLMatrixStack& _modelView ) { modelViewStack = &_modelView; }

	inline void SetProjectionMatrixStack(GLMatrixStack& _projection) { projectionStack = &_projection; }

	inline void SetMatrixStacks( GLMatrixStack& _modelView, GLMatrixStack& _projection )
	{
		modelViewStack = &_modelView;
		projectionStack = &_projection;
	}

	const Matrix4x4f& GetModelViewProjectionMatrix( void )
	{
		glsMatrixMultiply4x4( modelViewProjectionMatrix, projectionStack->GetMatrix(), modelViewStack->GetMatrix() );
		return modelViewProjectionMatrix;
	}

	inline const Matrix4x4f& GetModelViewMatrix( void ) { return modelViewStack->GetMatrix(); }
	inline const Matrix4x4f& GetProjectionMatrix( void ) { return projectionStack->GetMatrix(); }

	const Matrix3x3f& GetNormalMatrix( bool _normalize = false )
	{
		glsExtractRotationMatrix3x3( normalMatrix, GetModelViewMatrix() );

		if( _normalize ) {
			glsNormalizeVector3( &normalMatrix[0] );
			glsNormalizeVector3( &normalMatrix[3] );
			glsNormalizeVector3( &normalMatrix[6] );
		}

		return normalMatrix;
	}

protected:
	Matrix4x4f	modelViewProjectionMatrix;
	Matrix3x3f	normalMatrix;

	GLMatrixStack*  modelViewStack;
	GLMatrixStack* projectionStack;
};