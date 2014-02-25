/*
	File:		GLTriangleBatch.h
	Author:		Bob Pasekoff
	Updated:	7/15/2013

*/

#pragma once

#include "stdafx.h"
//#ifdef WIN32
//#include <windows.h>		// Must have for Windows platform builds
//#ifndef GLEW_STATIC
//#define GLEW_STATIC
//#endif

#include "gl\glew.h"			// OpenGL Extension "autoloader"
#include <gl\gl.h>			// Microsoft OpenGL headers (version 1.1 by themselves)
//#endif
//#include <glew.h>
//#pragma comment(lib,"GLEW32.lib")

#include "glsMath.h"
#include "GLBatchBase.h"
#include "GLShaderManager.h"

#define VERTEX_DATA		0
#define NORMAL_DATA		1
#define TEXTURE_DATA	2
#define JINDEX_DATA		4
#define JWEIGHT_DATA	3
#define	INDEX_DATA		5

typedef int	Vec4I[4];

class GLTriangleBatch : public GLBatchBase
{
public:
	GLTriangleBatch( void );
	virtual ~GLTriangleBatch( void );
	void ReleaseVBOs( void );

	// for adding triangles
	void BeginMesh( GLuint _maxVerts, GLuint _maxIndices );
	void AddTriangle( Vec3f _verts[3], Vec3f _norms[3], Vec2f _texCoords[3], GLubyte _jIndex[4] = nullptr, Vec4f _jWeight[3] = nullptr);
	void End( void );

	inline GLuint GetIndexCount( void )		{ return numIndices; }
	inline GLuint GetVertexCount( void )	{ return numVerts; }

	inline bool IsInitialized( void	)		{ return bInitialized; }

	virtual void Draw( void );

protected:
	GLushort*		pIndices;
	Vec3f*			pVerts;
	Vec3f*			pNorms;
	Vec2f*			pTexCoords;

	GLubyte*		pJIndex;
	Vec4f*			pJWeight;

	GLuint			maxIndices;
	GLuint			numIndices;
	GLuint			numVerts;

	GLuint			bufferObjects[6];
	GLuint			vertexArrayBufferObject;

	bool			bInitialized;
	bool			m_bAnimated;

private:

	GLTriangleBatch(const GLTriangleBatch &);
	GLTriangleBatch& operator=(const GLTriangleBatch &);

};