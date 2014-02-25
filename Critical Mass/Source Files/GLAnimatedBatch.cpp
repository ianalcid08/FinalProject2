#include "../Header Files/StdAfx.h"
#include "../Header Files/GLAnimatedBatch.h"

GLAnimatedBatch::GLAnimatedBatch(void)
{
	m_Indices = nullptr;
	m_Verts = nullptr;
	m_Norms = nullptr;
	m_TexCoords = nullptr;

	m_MaxIndices = 0;
	m_NumIndices = 0;
	m_NumVerts = 0;

	m_Initialized = false;
}

GLAnimatedBatch::~GLAnimatedBatch(void)
{
	delete [] m_Indices;
	delete [] m_Verts;
	delete [] m_Norms;
	delete [] m_TexCoords;
	delete [] m_JointIndex;

	glDeleteBuffers( 4, m_BufferObjects );

	glDeleteVertexArrays( 1, &m_VertexArrayBufferObject );
}

void GLAnimatedBatch::BeginMesh( GLuint _maxVerts )
{
	delete [] m_Indices;
	delete [] m_Verts;
	delete [] m_Norms;
	delete [] m_TexCoords;
	delete [] m_JointIndex;

	m_MaxIndices = _maxVerts;
	m_NumIndices = 0;
	m_NumVerts = 0;

	m_Indices = new GLushort[m_MaxIndices];
	m_Verts = new Vec3f[m_MaxIndices];
	m_Norms = new Vec3f[m_MaxIndices];
	m_TexCoords = new Vec2f[m_MaxIndices];
	m_JointIndex = new unsigned int[m_MaxIndices];
}

void GLAnimatedBatch::AddTriangle( Vec3f _verts[3], Vec3f _norms[3], Vec2f _texCoords[3], unsigned int* _jointIndex )
{
	const float e = 0.00001f;

	// Normalize all the normals
	glsNormalizeVector3( _norms[0] );
	glsNormalizeVector3( _norms[1] );
	glsNormalizeVector3( _norms[2] );


	for( GLuint counterVert = 0; counterVert < 3; ++counterVert )
	{
		GLuint counterMatch = 0;
		for( counterMatch = 0; counterMatch < m_NumVerts; ++counterMatch )
		{
			// If the positions are the same
			if( glsCloseEnough( m_Verts[counterMatch][0], _verts[counterVert][0], e ) &&
				glsCloseEnough( m_Verts[counterMatch][1], _verts[counterVert][1], e ) &&
				glsCloseEnough( m_Verts[counterMatch][2], _verts[counterVert][2], e ) &&

			// and the normal is the same
				glsCloseEnough( m_Norms[counterMatch][0], _norms[counterVert][0], e ) &&
				glsCloseEnough( m_Norms[counterMatch][1], _norms[counterVert][1], e ) &&
				glsCloseEnough( m_Norms[counterMatch][2], _norms[counterVert][2], e ) &&

			// and the texture coords are the same
				glsCloseEnough( m_TexCoords[counterMatch][0], _texCoords[counterVert][0], e ) &&
				glsCloseEnough( m_TexCoords[counterMatch][1], _texCoords[counterVert][1], e ) &&
				m_JointIndex[counterMatch] == *_jointIndex )
			{
				// add only the index
				m_Indices[m_NumIndices++] = counterMatch;
				break;
			}
		}

		// No match for this vert.  Add.
		if( counterMatch == m_NumVerts && m_NumVerts < m_MaxIndices && m_NumIndices < m_MaxIndices )
		{
			memcpy( m_Verts[m_NumVerts], _verts[counterVert], sizeof( Vec3f ) );
			memcpy( m_Norms[m_NumVerts], _norms[counterVert], sizeof( Vec3f ) );
			memcpy( m_TexCoords[m_NumVerts], _texCoords[counterVert], sizeof( Vec2f ) );
			m_Indices[m_NumIndices++] = m_NumVerts++;
		}
	}
}

void GLAnimatedBatch::End( void )
{
	m_Initialized = true;

	// Create master vertex array
	glGenVertexArrays( 1, &m_VertexArrayBufferObject );
	glBindVertexArray( m_VertexArrayBufferObject );
	
	// Create buffer objects
	glGenBuffers( 4, m_BufferObjects );

	// Copy data to video memory
	// vertex data
	glBindBuffer( GL_ARRAY_BUFFER, m_BufferObjects[VERTEX] );
	glEnableVertexAttribArray( GLS_ATTRIBUTE_VERTEX );
	glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat ) * m_NumVerts * 3, m_Verts, GL_STATIC_DRAW );
	glVertexAttribPointer( GLS_ATTRIBUTE_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0 );

	// normal data
	glBindBuffer( GL_ARRAY_BUFFER, m_BufferObjects[NORMAL] );
	glEnableVertexAttribArray( GLS_ATTRIBUTE_NORMAL );
	glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat ) * m_NumVerts * 3, m_Norms, GL_STATIC_DRAW );
	glVertexAttribPointer( GLS_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0 );

	// tex coords
	glBindBuffer( GL_ARRAY_BUFFER, m_BufferObjects[TEXTURE] );
	glEnableVertexAttribArray( GLS_ATTRIBUTE_TEXTURE0 );
	glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat ) * m_NumVerts * 2, m_TexCoords, GL_STATIC_DRAW );
	glVertexAttribPointer( GLS_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, 0 );

	// joint index
	glBindBuffer( GL_ARRAY_BUFFER, m_BufferObjects[JOINT] );
	glEnableVertexAttribArray( GLS_ATTRIBUTE_TEXTURE1 );
	glBufferData( GL_ARRAY_BUFFER, sizeof( GLuint ) * m_NumVerts, m_JointIndex, GL_STATIC_DRAW );
	glVertexAttribPointer( GLS_ATTRIBUTE_TEXTURE1, 1, GL_INT, GL_FALSE, 0, 0 );

	// indices
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_BufferObjects[INDEX] );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( GLushort ) * m_NumIndices, m_Indices, GL_STATIC_DRAW );

	// done5
	delete [] m_Indices;
	delete [] m_Verts;
	delete [] m_Norms;
	delete [] m_TexCoords;
	delete [] m_JointIndex;

	// mark pointers as unused
	m_Indices  = nullptr;
	m_Verts = nullptr;
	m_Norms = nullptr;
	m_TexCoords = nullptr;
	m_JointIndex = nullptr;

	glBindVertexArray( 0 );
}

void GLAnimatedBatch::Draw( void )
{
	glBindVertexArray( m_VertexArrayBufferObject );

	glDrawElements( GL_TRIANGLES, m_NumIndices, GL_UNSIGNED_SHORT, 0 );

	glBindVertexArray( m_VertexArrayBufferObject );
}