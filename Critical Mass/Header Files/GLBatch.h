/*
	File:		GLBatch.h
	Author:		Bob Pasekoff
	Updated:	7/16/2013

*/

#ifndef _GLBATCH_
#define _GLBATCH_

#include "gl\glew.h"			// OpenGL Extension "autoloader"
#include <gl\gl.h>			// Microsoft OpenGL headers (version 1.1 by themselves)

#include "glsMath.h"
#include "GLBatchBase.h"

class GLBatch : public GLBatchBase
{
public:
	GLBatch( void );
	virtual ~GLBatch( void );
	void ReleaseVBOs( void );

	// start populating the array
	void Begin( GLenum _primitive, GLuint _verts, GLuint _textureUnits = 0 );

	// tell batch you're done
	void End( void );

	// block-copy vertex data in
	void CopyVertexData3f( Vec3f *vVerts );
	void CopyNormalDataf( Vec3f *vNorms );
	void CopyColorData4f( Vec4f *vColors );
	void CopyTexCoordData2f( Vec2f *vTexCoords, GLuint uiTextureLayer );

	inline void CopyVertexData3f( GLfloat *vVerts ) { CopyVertexData3f( (Vec3f *)(vVerts) ); }
	inline void CopyNormalDataf( GLfloat *vNorms ) { CopyNormalDataf( (Vec3f *)(vNorms) ); }
	inline void CopyColorData4f( GLfloat *vColors ) { CopyColorData4f( (Vec4f *)(vColors) ); }
	inline void CopyTexCoordData2f( GLfloat *vTex, GLuint uiTextureLayer ) { CopyTexCoordData2f( (Vec2f *)(vTex), uiTextureLayer ); }


	virtual void Draw( void );

	// Immediate Mode Emulator
	// Try not to use any of the following
	void Reset( void );

	void Vertex3f( GLfloat _x, GLfloat _y, GLfloat _z );
	void Vertex3fv( Vec3f _vertex );

	void Normal3f( GLfloat _x, GLfloat _y, GLfloat _z );
	void Normal3fv( Vec3f _normal );

	void Color4f( GLfloat _r, GLfloat _g, GLfloat _b, GLfloat _a );
	void Color4fv( Vec4f _color );

	void MultiTexCoord2f( GLuint _texture, GLclampf _u, GLclampf _v );
	void MultiTexCoord2fv( GLuint _texture, Vec2f _texCoord );

protected:
	GLenum		primitiveType;
	
	GLuint		vertexArray;
	GLuint		normalArray;
	GLuint		colorArray;
	GLuint*		textureCoordArray;
	GLuint		vertexArrayObject;

	GLuint		vertsBuilding;		// vertex counter ( IME )
	GLuint		numVerts;			// verts in this batch
	GLuint		numTextureUnits;	// how many sets of UVs

	bool		batchDone;

	Vec3f*		pVerts;
	Vec3f*		pNormals;
	Vec4f*		pColors;
	Vec2f**		pTexCoords;
};


#endif  //_GLBATCH_