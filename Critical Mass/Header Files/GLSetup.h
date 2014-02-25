/*
	File:		GLSetup.h
	Author:		Bob Pasekoff
	Updated:	7/11/2013

*/

#pragma once

// Maximum size of the static shader memory block
// Intended to prevent heap fragmentation
#define MAX_SHADER_LENGTH 8192


//#ifdef WIN32
//#include <windows.h>		// Must have for Windows platform builds
//#ifndef GLEW_STATIC
//#define GLEW_STATIC
//#endif

#include "gl\glew.h"			// OpenGL Extension "autoloader"
#include <gl\gl.h>			// Microsoft OpenGL headers (version 1.1 by themselves)
//#endif

//#include <glew.h>
//#include <GL/GL.h>
//#pragma comment(lib,"glu32.lib")

#include "glsMath.h"

class GLBatch;
class GLTriangleBatch;

// Get the OpenGL Version
void glsGetOpenGLVersion( GLint& _major, GLint& _minor );

// Load a .BMP file
GLbyte* glsReadBMPBits( const char* _fileName, int* _pWidth, int* _pHeight );

// Load a .TGA file
GLbyte* glsReadTGABits( const char* _fileName, GLint* _width , GLint* _height, GLint* _components, GLenum* _format );

// Load a .PNG file
//	This functionality will probably want to get moved into the asset manager.
//void glsReadPNGBits( const char* _fileName, GLint* _width, GLint* _height /* params */ );

// Create debug render objects
void glsCreateSphere( GLTriangleBatch& _sphereBatch, GLfloat _radius );
void glsCreateCube( GLBatch& _cubeBatch, GLfloat _radius );

// Load Shaders
//	This functionality will probably want to get moved into the asset manager.
void glsLoadShaderSrc( const char *_src, GLuint _shader );
bool glsLoadShaderFile( const char* _fileName, GLuint _shader );

GLuint glsLoadShaderPair( const char* _vertexShader, const char* _pixelShader );
GLuint glsLoadShaderPairWithAttributes( const char* _vertexShader, const char* _pixelShader, ... );

GLuint glsLoadShaderPairSrc( const char* _vertexSrc, const char* pixelSrc );
GLuint glsLoadShaderPairSrcWithAttributes( const char* _vertexShader, const char* _pixelShader, ... );

// Check for Errors
bool glsCheckShaderErrors( GLuint _shaderName = 0 );

// Create a matrix that maps geometry to the screen
// 1 unit = 1px on the x and/or y axes
void glsCreateOrtho2DMatrix( GLuint _screenWidth, GLuint _screenHeight, Matrix4x4f& orthoMatrix, GLBatch& screenQuad );


bool _glsCheckGLErrors( void );

#ifdef _DEBUG
#define glsCheckGLErrors _glsCheckGLErrors
#else
#define glsCheckGLErrors __noop
#endif