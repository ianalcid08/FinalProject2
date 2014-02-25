/*
	File:		GLTriangleBatch.cpp
	Author:		Bob Pasekoff
	Updated:	7/15/2013

*/

#include "../Header Files/stdafx.h"
#include "../Header Files/GLTriangleBatch.h"

GLTriangleBatch::GLTriangleBatch( void )
{
	pIndices = nullptr;
	pVerts = nullptr;
	pNorms = nullptr;
	pTexCoords = nullptr;
	pJIndex = nullptr;
	pJWeight = nullptr;

	maxIndices = 0;
	numIndices = 0;
	numVerts = 0;

	vertexArrayBufferObject = -1;
	
	m_bAnimated = false;
	bInitialized = false;
}

GLTriangleBatch::~GLTriangleBatch( void )
{
	ReleaseVBOs();
}

void GLTriangleBatch::ReleaseVBOs( void )
{
	// This is just in case.
	delete [] pIndices;
	delete [] pVerts;
	delete [] pNorms;
	delete [] pTexCoords;
	delete [] pJIndex;
	delete [] pJWeight;

	glDeleteBuffers( 6, bufferObjects );

	glDeleteVertexArrays( 1, &vertexArrayBufferObject );
}

void GLTriangleBatch::BeginMesh( GLuint _maxVerts, GLuint _maxIndices )
{
	// This is also just in case.
	delete [] pIndices;
	delete [] pVerts;
	delete [] pNorms;
	delete [] pTexCoords;
	delete [] pJIndex;
	delete [] pJWeight;

	ReleaseVBOs();

	maxIndices = _maxIndices;
	numIndices = 0;
	numVerts = 0;

	pJIndex = new GLubyte[_maxVerts * 4];
	pJWeight = new Vec4f[_maxVerts];

	pIndices = new GLushort[maxIndices];
	pVerts = new Vec3f[_maxVerts];
	pNorms = new Vec3f[_maxVerts];
	pTexCoords = new Vec2f[_maxVerts];
}

// Search for (almost) identical verts.  If one is found, add it to the index array.
// Otherwise, add to both the index array and the vertex array.
void GLTriangleBatch::AddTriangle( Vec3f _verts[3], Vec3f _norms[3], Vec2f _texCoords[3], GLubyte _jIndex[12], Vec4f _jWeight[3] )
{
const float e = 0.00001f;

	// Normalize all the normals
	glsNormalizeVector3( _norms[0] );
	glsNormalizeVector3( _norms[1] );
	glsNormalizeVector3( _norms[2] );
	
	for( GLuint counterVert = 0; counterVert < 3; ++counterVert )
	{
		GLuint counterMatch = 0;
		for( counterMatch = 0; counterMatch < numVerts; ++counterMatch )
		{
			// If the positions are the same
			if( glsCloseEnough( pVerts[counterMatch][0], _verts[counterVert][0], e ) &&
				glsCloseEnough( pVerts[counterMatch][1], _verts[counterVert][1], e ) &&
				glsCloseEnough( pVerts[counterMatch][2], _verts[counterVert][2], e ) &&

			// and the normal is the same
				glsCloseEnough( pNorms[counterMatch][0], _norms[counterVert][0], e ) &&
				glsCloseEnough( pNorms[counterMatch][1], _norms[counterVert][1], e ) &&
				glsCloseEnough( pNorms[counterMatch][2], _norms[counterVert][2], e ) &&

			// and the texture coords are the same
				glsCloseEnough( pTexCoords[counterMatch][0], _texCoords[counterVert][0], e ) &&
				glsCloseEnough( pTexCoords[counterMatch][1], _texCoords[counterVert][1], e ) )
			{
				// add only the index
				pIndices[numIndices] = counterMatch;
				++numIndices;
				break;
			}
		}

		// No match for this vert.  Add.
		if( counterMatch == numVerts && numVerts < maxIndices && numIndices < maxIndices )
		{
			memcpy( pVerts[numVerts], _verts[counterVert], sizeof( Vec3f ) );
			memcpy( pNorms[numVerts], _norms[counterVert], sizeof( Vec3f ) );
			memcpy( pTexCoords[numVerts], _texCoords[counterVert], sizeof( Vec2f ) );
			
			// HAX - Have to reset the JIndices to null so the buffers are aligned properly.
			if( _jIndex != nullptr )
				memcpy( &pJIndex[numVerts * 4], &_jIndex[counterVert * 4], sizeof(GLubyte) * 4 );
			else if( pJIndex != nullptr )
			{
				delete [] pJIndex;
				pJIndex = nullptr;
			}
			
			// HAX - Have to reset the JIndices to null so the buffers are aligned properly.
			if( _jWeight != nullptr )
				memcpy( &pJWeight[numVerts], &_jWeight[counterVert], sizeof(Vec4f));
			else if( pJWeight != nullptr )
			{
				delete [] pJWeight;
				pJWeight = nullptr;
			}

			pIndices[numIndices] = numVerts;
			++numIndices;
			++numVerts;
		}
	}
}


void GLTriangleBatch::End( void )
{
	bInitialized = true;

	// Create master vertex array
	glGenVertexArrays( 1, &vertexArrayBufferObject );
	glBindVertexArray( vertexArrayBufferObject );
	
	// Create buffer objects
	if( pJIndex )
		glGenBuffers( 6, bufferObjects );
	else
		glGenBuffers( 4, bufferObjects );

	// Copy data to video memory%
	// vertex data
	glBindBuffer( GL_ARRAY_BUFFER, bufferObjects[VERTEX_DATA] );
	glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat ) * numVerts * 3, pVerts, GL_STATIC_DRAW );
	glVertexAttribPointer( GLS_ATTRIBUTE_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray( GLS_ATTRIBUTE_VERTEX );

	// normal data
	glBindBuffer( GL_ARRAY_BUFFER, bufferObjects[NORMAL_DATA] );
	
	glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat ) * numVerts * 3, pNorms, GL_STATIC_DRAW );
	glVertexAttribPointer( GLS_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray( GLS_ATTRIBUTE_NORMAL );

	// tex coords
	glBindBuffer( GL_ARRAY_BUFFER, bufferObjects[TEXTURE_DATA] );
	
	glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat ) * numVerts * 2, pTexCoords, GL_STATIC_DRAW );
	glVertexAttribPointer( GLS_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray( GLS_ATTRIBUTE_TEXTURE0 );
	if( pJIndex )
	{
		m_bAnimated = true;
		// Joint index data
		glBindBuffer( GL_ARRAY_BUFFER, bufferObjects[JINDEX_DATA] );
	
		// Only derefrence if the joint indices are valid.
		glBufferData( GL_ARRAY_BUFFER, sizeof( GLubyte ) * (4 * numVerts), (GLvoid*)pJIndex, GL_STATIC_DRAW );

		glVertexAttribIPointer( GLS_ATTRIBUTE_JINDICES, 4, GL_BYTE, 0, 0 );
		glEnableVertexAttribArray( GLS_ATTRIBUTE_JINDICES );
	}
	
	if( pJWeight )
	{
	// Joint Weight data
		glBindBuffer( GL_ARRAY_BUFFER, bufferObjects[JWEIGHT_DATA] );
	
		// Only derefrence if the weights are valid.
		glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat ) * (4 * numVerts), (GLvoid*)pJWeight, GL_STATIC_DRAW );

		glVertexAttribPointer( GLS_ATTRIBUTE_WEIGHTS, 4, GL_FLOAT, GL_FALSE, 0, 0 );
		glEnableVertexAttribArray( GLS_ATTRIBUTE_WEIGHTS );
	}

	// indices
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bufferObjects[m_bAnimated ? INDEX_DATA : 3] );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( GLushort ) * numIndices, pIndices, GL_STATIC_DRAW );

	// done
	delete [] pIndices;
	delete [] pVerts;
	delete [] pNorms;
	delete [] pTexCoords;

	delete [] pJIndex;
	delete [] pJWeight;

	// mark pointers as unused
	pIndices  = nullptr;
	pVerts = nullptr;
	pNorms = nullptr;
	pTexCoords = nullptr;
	pJIndex = nullptr;
	pJWeight = nullptr;

	glBindVertexArray( 0 );
}


void GLTriangleBatch::Draw( void )
{
	glBindVertexArray( vertexArrayBufferObject );

	glEnableVertexAttribArray( GLS_ATTRIBUTE_VERTEX );
	glEnableVertexAttribArray( GLS_ATTRIBUTE_NORMAL );
	glEnableVertexAttribArray( GLS_ATTRIBUTE_TEXTURE0 );

	if(m_bAnimated)
	{
		glEnableVertexAttribArray( GLS_ATTRIBUTE_JINDICES );
		glEnableVertexAttribArray( GLS_ATTRIBUTE_WEIGHTS );
	}

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0 );

	if(m_bAnimated)
	{
		glDisableVertexAttribArray( GLS_ATTRIBUTE_JINDICES );
		glDisableVertexAttribArray( GLS_ATTRIBUTE_WEIGHTS );
	}

	glDisableVertexAttribArray( GLS_ATTRIBUTE_VERTEX );
	glDisableVertexAttribArray( GLS_ATTRIBUTE_NORMAL );
	glDisableVertexAttribArray( GLS_ATTRIBUTE_TEXTURE0 );

	// This may be unnecessary.
	//glBindVertexArray( 0 );
}