#ifndef _GLANIMBATCH
#define _GLANIMBATCH

#include "gl\glew.h"			// OpenGL Extension "autoloader"
#include <gl\gl.h>			// Microsoft OpenGL headers (version 1.1 by themselves)

#include "GLBatchBase.h"
#include "glsMath.h"
#include "GLShaderManager.h"

enum Data { VERTEX = 0, NORMAL, TEXTURE, JOINT, INDEX };

class GLAnimatedBatch : public GLBatchBase
{

public:

	GLAnimatedBatch( void );
	virtual ~GLAnimatedBatch( void );

	// for adding triangles
	void BeginMesh( GLuint _maxVerts );
	void AddTriangle( Vec3f _verts[3], Vec3f _norms[3], Vec2f _texCoords[3], unsigned int* _jointIndex );
	void End( void );

	inline GLuint GetIndexCount( void )		{ return m_NumIndices; }
	inline GLuint GetVertexCount( void )	{ return m_NumVerts; }
	
	inline bool IsInitialized( void	)		{ return m_Initialized; }

	virtual void Draw( void );

protected:

	//Matrix4x4f*		

	GLushort*		m_Indices;
	Vec3f*			m_Verts;
	Vec3f*			m_Norms;
	Vec2f*			m_TexCoords;
	GLuint*			m_JointIndex;

	GLuint			m_MaxIndices;
	GLuint			m_NumIndices;
	GLuint			m_NumVerts;

	GLuint			m_BufferObjects[4];
	GLuint			m_VertexArrayBufferObject;

	bool			m_Initialized;
};

#endif
