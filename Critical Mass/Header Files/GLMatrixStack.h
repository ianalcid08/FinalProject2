/*
File:		GLMatrixStack
Author:		Bob Pasekoff
Updated:	7/16/2013

*/

#include "../Header Files/stdafx.h"
#include "../Header Files/GLSetup.h"
#include "../Header Files/glsMath.h"
#include "../Header Files/GLFrame.h"

enum GLS_STACK_ERROR { GLS_STACK_NOERROR = 0, GLS_STACK_OVERFLOW, GLS_STACK_UNDERFLOW }; 

class GLMatrixStack
{
public:
	GLMatrixStack( int _stackDepth = 64 )
	{
		stackDepth = _stackDepth;
		pStack = new Matrix4x4f[_stackDepth];
		stackPointer = 0;
		glsLoadIdentity4x4( pStack[0] );
		lastError = GLS_STACK_NOERROR;
	}


	~GLMatrixStack( void )
	{
		delete [] pStack;
	}


	inline void LoadIdentity( void )
	{ 
		glsLoadIdentity4x4( pStack[stackPointer] ); 
	}

	inline void LoadMatrix( const Matrix4x4f _matrix )
	{ 
		glsCopyMatrix4x4( pStack[stackPointer], _matrix ); 
	}

	inline void LoadMatrix( GLFrame& _frame )
	{
		Matrix4x4f m;
		_frame.GetMatrix( m );
		LoadMatrix( m );
	}

	inline void MultMatrix( const Matrix4x4f _matrix )
	{
		Matrix4x4f mTemp;
		glsCopyMatrix4x4( mTemp, pStack[stackPointer] );
		glsMatrixMultiply4x4( pStack[stackPointer], mTemp, _matrix );
	}

	inline void MultMatrix( GLFrame& _frame )
	{
		Matrix4x4f m;
		_frame.GetMatrix( m );
		MultMatrix( m );
	}

	inline void PushMatrix( void )
	{
		if( stackPointer < stackDepth )
		{
			stackPointer++;
			glsCopyMatrix4x4( pStack[stackPointer], pStack[stackPointer-1] );
		}
		else
			lastError = GLS_STACK_OVERFLOW;
	}

	inline void PopMatrix( void )
	{
		if( stackPointer > 0 )
			stackPointer--;
		else
			lastError = GLS_STACK_UNDERFLOW;
	}

	void Scale( GLfloat _x, GLfloat _y, GLfloat _z )
	{
		Matrix4x4f mTemp, mScale;
		glsScaleMatrix4x4( mScale, _x, _y, _z );
		glsCopyMatrix4x4( mTemp, pStack[stackPointer] );
		glsMatrixMultiply4x4( pStack[stackPointer], mTemp, mScale );
	}


	void Translate( GLfloat _x, GLfloat _y, GLfloat _z )
	{
		Matrix4x4f mTemp, mScale;
		glsTranslationMatrix4x4( mScale, _x, _y, _z );
		glsCopyMatrix4x4( mTemp, pStack[stackPointer] );
		glsMatrixMultiply4x4( pStack[stackPointer], mTemp, mScale );			
	}

	void Rotate( GLfloat _angle, GLfloat _x, GLfloat _y, GLfloat _z )
	{
		Matrix4x4f mTemp, mRotate;
		glsRotationMatrix4x4( mRotate, float(glsDegToRad( _angle )), _x, _y, _z );
		glsCopyMatrix4x4( mTemp, pStack[stackPointer] );
		glsMatrixMultiply4x4( pStack[stackPointer], mTemp, mRotate );
	}

	void Scalev( const Vec3f _scale )
	{
		Matrix4x4f mTemp, mScale;
		glsScaleMatrix4x4( mScale, _scale );
		glsCopyMatrix4x4( mTemp, pStack[stackPointer] );
		glsMatrixMultiply4x4( pStack[stackPointer], mTemp, _scale );
	}


	void Translatev( const Vec3f _translate )
	{
		Matrix4x4f mTemp, mTranslate;
		glsLoadIdentity4x4( mTranslate );
		glsSetMatrixColumn4x4( mTranslate, _translate, 3 );
		glsCopyMatrix4x4( mTemp, pStack[stackPointer] );
		glsMatrixMultiply4x4( pStack[stackPointer], mTemp, mTranslate );
	}


	void Rotatev( GLfloat _angle, Vec3f _axis )
	{
		Matrix4x4f mTemp, mRotation;
		glsRotationMatrix4x4( mRotation, float(glsDegToRad( _angle )), _axis[0], _axis[1], _axis[2] );
		glsCopyMatrix4x4( mTemp, pStack[stackPointer] );
		glsMatrixMultiply4x4( pStack[stackPointer], mTemp, mRotation );
	}


	void PushMatrix( const Matrix4x4f _matrix )
	{
		if( stackPointer < stackDepth )
		{
			stackPointer++;
			glsCopyMatrix4x4( pStack[stackPointer], _matrix );
		}
		else
			lastError = GLS_STACK_OVERFLOW;
	}

	void PushMatrix( GLFrame& _frame )
	{
		Matrix4x4f m;
		_frame.GetMatrix( m );
		PushMatrix( m );
	}

	// Two different ways to get the matrix
	const Matrix4x4f& GetMatrix( void ) { return pStack[stackPointer]; }
	void GetMatrix( Matrix4x4f _matrix ) { glsCopyMatrix4x4( _matrix, pStack[stackPointer] ); }


	inline GLS_STACK_ERROR GetLastError( void )
	{
		GLS_STACK_ERROR retval = lastError;
		lastError = GLS_STACK_NOERROR;
		return retval; 
	}

protected:
	GLS_STACK_ERROR		lastError;
	int					stackDepth;
	int					stackPointer;
	Matrix4x4f			*pStack;
};