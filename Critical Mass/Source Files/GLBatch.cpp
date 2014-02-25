/*
	File:		GLBatch.cpp
	Author:		Bob Pasekoff
	Updated:	7/15/2013

*/

#include "../Header Files/stdafx.h"
#include "../Header Files/GLBatch.h"
#include "../Header Files/GLShaderManager.h"

GLBatch::GLBatch( void ) : numTextureUnits( 0 ), numVerts( 0 ), pVerts( nullptr ),
	pNormals( nullptr ), pColors( nullptr ), vertexArray( 0 ), normalArray( 0 ),
	colorArray( 0 ), vertexArrayObject( 0 ), batchDone( false ), vertsBuilding( 0 ),
	textureCoordArray( nullptr ), pTexCoords( nullptr )
{
}

GLBatch::~GLBatch( void )
{
	ReleaseVBOs();
}

void GLBatch::ReleaseVBOs( void )
{
	// VBOs

	glDeleteBuffers( 1, &vertexArray );

	glDeleteBuffers( 1, &normalArray );

	glDeleteBuffers( 1, &colorArray );

	glDeleteVertexArrays( 1, &vertexArrayObject );

	if( textureCoordArray )
	{
		for( unsigned int i = 0; i < numTextureUnits; ++i )
		{
			glDeleteBuffers( 1, &textureCoordArray[i] );
		}

		delete [] textureCoordArray;
		textureCoordArray = nullptr;
	}

	if( pTexCoords )
	{
		delete [] pTexCoords;
		pTexCoords = nullptr;
	}
}

void GLBatch::Begin( GLenum _primitive, GLuint _verts, GLuint _textureUnits )
{
	ReleaseVBOs();

	primitiveType = _primitive;
	numVerts = _verts;

	// no more than 4 sets of UVs
	if( _textureUnits > 4 )
		_textureUnits = 4;

	numTextureUnits = _textureUnits;

	if( numTextureUnits )
	{
		textureCoordArray = new GLuint[numTextureUnits];

		pTexCoords = new Vec2f*[numTextureUnits];
		for( unsigned int i = 0; i < numTextureUnits; ++i )
		{
			textureCoordArray[i] = 0;
			pTexCoords[i] = nullptr;
		}
	}

	glGenVertexArrays( 1, &vertexArrayObject );
	glBindVertexArray( vertexArrayObject );
}


void GLBatch::End( void )
{
	if( pVerts )
	{
		glBindBuffer( GL_ARRAY_BUFFER, vertexArray );
		glUnmapBuffer( GL_ARRAY_BUFFER );
		pVerts = nullptr;
	}

	if( pColors )
	{
		glBindBuffer( GL_ARRAY_BUFFER, colorArray );
		glUnmapBuffer( GL_ARRAY_BUFFER );
		pColors = nullptr;
	}

	if( pNormals )
	{
		glBindBuffer( GL_ARRAY_BUFFER, normalArray );
		glUnmapBuffer( GL_ARRAY_BUFFER );
		pNormals = nullptr;
	}

	for( unsigned int i = 0; i < numTextureUnits; ++i )
	{
		if( pTexCoords[i] )
		{
			glBindBuffer( GL_ARRAY_BUFFER, textureCoordArray[i] );
			glUnmapBuffer( GL_ARRAY_BUFFER );
			pTexCoords[i] = nullptr;
		}
	}

	glBindVertexArray( vertexArrayObject );

	if( vertexArray )
	{
		glEnableVertexAttribArray( GLS_ATTRIBUTE_VERTEX );
		glBindBuffer( GL_ARRAY_BUFFER, vertexArray );
		glVertexAttribPointer( GLS_ATTRIBUTE_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	}

	if( colorArray )
	{
		glEnableVertexAttribArray( GLS_ATTRIBUTE_COLOR );
		glBindBuffer( GL_ARRAY_BUFFER, colorArray );
		glVertexAttribPointer( GLS_ATTRIBUTE_COLOR, 4, GL_FLOAT, GL_FALSE, 0, 0 );
	}

	if( normalArray )
	{
		glEnableVertexAttribArray( GLS_ATTRIBUTE_NORMAL );
		glBindBuffer( GL_ARRAY_BUFFER, normalArray );
		glVertexAttribPointer( GLS_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	}

	// How many texture units
	for( unsigned int i = 0; i < numTextureUnits; i++ )
		if( textureCoordArray[i] != 0 ) {
			glEnableVertexAttribArray( GLS_ATTRIBUTE_TEXTURE0 + i ),
				glBindBuffer( GL_ARRAY_BUFFER, textureCoordArray[i] );
			glVertexAttribPointer( GLS_ATTRIBUTE_TEXTURE0 + i, 2, GL_FLOAT, GL_FALSE, 0, 0 );
		}

	batchDone = true;
	glBindVertexArray( 0 );
}


void GLBatch::CopyVertexData3f( Vec3f* _verts )
{
	if( !vertexArray ) // create the buffer object, allocate space
	{
		glGenBuffers( 1, &vertexArray );
		glBindBuffer( GL_ARRAY_BUFFER, vertexArray );
		glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat ) * 3 * numVerts, _verts, GL_DYNAMIC_DRAW );
	}
	else // use the existing buffer object
	{
		glBindBuffer( GL_ARRAY_BUFFER, vertexArray );

		// copy the data in
		glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof( GLfloat ) * 3 * numVerts, _verts );
		pVerts = nullptr;
	}
}

void GLBatch::CopyNormalDataf( Vec3f* _norms )
{
	if( !normalArray ) // create the buffer object, allocate space
	{
		glGenBuffers( 1, &normalArray );
		glBindBuffer( GL_ARRAY_BUFFER, normalArray );
		glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat ) * 3 * numVerts, _norms, GL_DYNAMIC_DRAW );
	}
	else // use the existing buffer object
	{
		glBindBuffer( GL_ARRAY_BUFFER, normalArray );

		// copy the data in
		glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof( GLfloat ) * 3 * numVerts, _norms );
		pNormals = nullptr;
	}
}

void GLBatch::CopyColorData4f( Vec4f* _colors )
{
	if( !colorArray ) // create the buffer object, allocate space
	{
		glGenBuffers( 1, &colorArray );
		glBindBuffer( GL_ARRAY_BUFFER, colorArray );
		glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat ) * 4 * numVerts, _colors, GL_DYNAMIC_DRAW );
	}
	else // use the existing buffer structure
	{
		glBindBuffer( GL_ARRAY_BUFFER, colorArray );

		// copy the data in
		glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof( GLfloat ) * 4 * numVerts, _colors );
		pColors = nullptr;
	}
}

void GLBatch::CopyTexCoordData2f( Vec2f* _texCoords, GLuint _textureLayer )
{
	if( !textureCoordArray[_textureLayer] ) // create the buffer object, allocate space
	{
		glGenBuffers( 1, &textureCoordArray[_textureLayer] );
		glBindBuffer( GL_ARRAY_BUFFER, textureCoordArray[_textureLayer] );
		glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat ) * 2 * numVerts, _texCoords, GL_DYNAMIC_DRAW );
	}
	else // use the existing buffer structure
	{
		glBindBuffer( GL_ARRAY_BUFFER, textureCoordArray[_textureLayer] );

		// copy the data in
		glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof( GLfloat ) * 2 * numVerts, _texCoords );
		pTexCoords[_textureLayer] = nullptr;
	}
}

void GLBatch::Draw( void )
{
	if( !batchDone )
		return;

	glBindVertexArray( vertexArrayObject );

	glDrawArrays( primitiveType, 0, numVerts );

	glBindVertexArray( 0 );
}

void GLBatch::Reset( void )
{
	batchDone = false;
	vertsBuilding = 0;
}

void GLBatch::Vertex3f( GLfloat _x, GLfloat _y, GLfloat _z )
{
	if( !vertexArray )
	{
		glGenBuffers( 1, &vertexArray );
		glBindBuffer( GL_ARRAY_BUFFER, vertexArray );
		glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat ) * 3 * numVerts, nullptr, GL_DYNAMIC_DRAW );
	}

	if( !pVerts )
	{
		glBindBuffer( GL_ARRAY_BUFFER, vertexArray );
		pVerts = (Vec3f*)glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );
	}

	if( vertsBuilding >= numVerts )
		return;

	pVerts[vertsBuilding][0] = _x;
	pVerts[vertsBuilding][1] = _y;
	pVerts[vertsBuilding][2] = _z;
	++vertsBuilding;
}

void GLBatch::Vertex3fv( Vec3f _vertex )
{
	if( !vertexArray )
	{
		glGenBuffers( 1, &vertexArray );
		glBindBuffer( GL_ARRAY_BUFFER, vertexArray );
		glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat ) * 3 * numVerts, nullptr, GL_DYNAMIC_DRAW );
	}

	if( !pVerts )
	{
		glBindBuffer( GL_ARRAY_BUFFER, vertexArray );
		pVerts = (Vec3f*)glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );
	}

	if( vertsBuilding >= numVerts )
		return;

	memcpy( pVerts[vertsBuilding], _vertex, sizeof( Vec3f ) );
	++vertsBuilding;
}

void GLBatch::Normal3f( GLfloat _x, GLfloat _y, GLfloat _z )
{
	if( !normalArray )
	{
		glGenBuffers( 1, &normalArray );
		glBindBuffer( GL_ARRAY_BUFFER, normalArray );
		glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat) * 3 * numVerts, nullptr, GL_DYNAMIC_DRAW );
	}

	if( !pNormals )
	{
		glBindBuffer( GL_ARRAY_BUFFER, normalArray );
		pNormals = (Vec3f*)glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );
	}

	if( vertsBuilding >= numVerts )
		return;

	pNormals[vertsBuilding][0] = _x;
	pNormals[vertsBuilding][1] = _y;
	pNormals[vertsBuilding][2] = _z;
}

void GLBatch::Normal3fv( Vec3f _normal )
{
	if( !normalArray )
	{
		glGenBuffers( 1, &normalArray );
		glBindBuffer( GL_ARRAY_BUFFER, normalArray );
		glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat ) * 3 * numVerts, nullptr, GL_DYNAMIC_DRAW );
	}

	if( !pNormals )
	{
		glBindBuffer( GL_ARRAY_BUFFER, normalArray );
		pNormals = (Vec3f*)glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );
	}

	if( vertsBuilding >= numVerts )
		return;

	memcpy( pNormals[vertsBuilding], _normal, sizeof( Vec3f ) );
}

void GLBatch::Color4f( GLfloat _r, GLfloat _g, GLfloat _b, GLfloat _a )
{
	if( !colorArray )
	{
		glGenBuffers( 1, &colorArray );
		glBindBuffer( GL_ARRAY_BUFFER, colorArray );
		glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat ) * 4 * numVerts, nullptr, GL_DYNAMIC_DRAW );
	}

	if( !pColors )
	{
		glBindBuffer( GL_ARRAY_BUFFER, colorArray );
		pColors = (Vec4f*)glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );
	}

	if( vertsBuilding >= numVerts )
		return;

	pColors[vertsBuilding][0] = _r;
	pColors[vertsBuilding][1] = _g;
	pColors[vertsBuilding][2] = _b;
	pColors[vertsBuilding][3] = _a;
}

void GLBatch::Color4fv( Vec4f _color )
{
	if( !colorArray )
	{
		glGenBuffers( 1, &colorArray );
		glBindBuffer( GL_ARRAY_BUFFER, colorArray );
		glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat ) * 4 * numVerts, nullptr, GL_DYNAMIC_DRAW );
	}

	if( !pColors )
	{
		glBindBuffer( GL_ARRAY_BUFFER, colorArray );
		pColors = (Vec4f*)glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );
	}

	if( vertsBuilding >= numVerts )
		return;

	memcpy( pColors[vertsBuilding], _color, sizeof( Vec4f ) );
}

void GLBatch::MultiTexCoord2f( GLuint _texture, GLclampf _u, GLclampf _v )
{
	if( !textureCoordArray[_texture] )
	{
		glGenBuffers( 1, &textureCoordArray[_texture] );
		glBindBuffer( GL_ARRAY_BUFFER, textureCoordArray[_texture] );
		glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat ) * 2 * numVerts, nullptr, GL_DYNAMIC_DRAW );
	}

	if( !pTexCoords[_texture] )
	{
		glBindBuffer( GL_ARRAY_BUFFER, textureCoordArray[_texture] );
		pTexCoords[_texture] = (Vec2f*)glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );
	}

	if( vertsBuilding >= numVerts )
		return;

	pTexCoords[_texture][vertsBuilding][0] = _u;
	pTexCoords[_texture][vertsBuilding][1] = _v;
}

void GLBatch::MultiTexCoord2fv( GLuint _texture, Vec2f _texCoord )
{
	if( !textureCoordArray[_texture] )
	{
		glGenBuffers( 1, &textureCoordArray[_texture] );
		glBindBuffer( GL_ARRAY_BUFFER, textureCoordArray[_texture] );
		glBufferData( GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * numVerts, nullptr, GL_DYNAMIC_DRAW );
	}

	if( !pTexCoords[_texture] )
	{
		glBindBuffer( GL_ARRAY_BUFFER, textureCoordArray[_texture] );
		pTexCoords[_texture] = (Vec2f*)glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );
	}

	if( vertsBuilding >= numVerts )
		return;

	memcpy( pTexCoords[_texture], _texCoord, sizeof( Vec2f ) );
}