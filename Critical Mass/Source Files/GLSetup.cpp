/*
	File:		GLSetup.cpp
	Author:		Bob Pasekoff
	Updated:	7/11/2013

*/

#include "../Header Files/stdafx.h"
#include "../Header Files/GLSetup.h"
#include "../Header Files/glsMath.h"
#include "../Header Files/GLBatch.h"
#include "../Header Files/GLTriangleBatch.h"

// Get the OpenGL Version
void glsGetOpenGLVersion( GLint& _major, GLint& _minor )
{
	glGetIntegerv( GL_MAJOR_VERSION, &_major );
	glGetIntegerv( GL_MINOR_VERSION, &_minor );
}


// Load a .BMP file (for debugging only, please)
#pragma pack(1)
struct RGB
{ 
	GLbyte blue;
	GLbyte green;
	GLbyte red;
	GLbyte alpha;
};

struct BMPInfoHeader
{
	GLuint		size;
	GLuint		width;
	GLuint		height;
	GLushort	planes;
	GLushort	bits;
	GLuint		compression;
	GLuint		imageSize;
	GLuint		xScale;
	GLuint		yScale;
	GLuint		colors;
	GLuint		importantColors;
};

struct BMPHeader
{
	GLushort	type; 
	GLuint		size; 
	GLushort	unused; 
	GLushort	unused2; 
	GLuint		offset; 
}; 

struct BMPInfo
{
	BMPInfoHeader	header;
	RGB				colors[1];
};
#pragma pack(8)

GLbyte* glsReadBMPBits( const char* _fileName, int* _pWidth, int* _pHeight )
{
	FILE*			pFile;
	BMPInfo*		pBMPInfo = nullptr;
	unsigned long	infoSize = 0;
	unsigned long	bitSize = 0;
	GLbyte*			pBits = nullptr;
	BMPHeader		bitmapHeader;
	
	// Try to open the file
	//pFile = fopen( _fileName, "rb" );
	fopen_s( &pFile, _fileName, "rb" );
	if( !pFile )
		return nullptr;

	// Read header info
	fread( &bitmapHeader, sizeof( BMPHeader ), 1, pFile );

	// Read BMP info
	infoSize = bitmapHeader.offset - sizeof( BMPHeader );
	pBMPInfo = (BMPInfo *) malloc( sizeof( GLbyte ) * infoSize );
	if( 1 != fread( pBMPInfo, infoSize, 1, pFile) )
	{
		free( pBMPInfo );
		fclose( pFile );
		return nullptr;
	}

	// Grab the dimensions of the bitmap
	*_pWidth = pBMPInfo->header.width;
	*_pHeight = pBMPInfo->header.height;
	bitSize = pBMPInfo->header.imageSize;

	// If necessary, calculate the size
	if( 24 != pBMPInfo->header.bits )
	{
		free( pBMPInfo );
		return nullptr;
	}

	if( !bitSize )
	{
		bitSize = (*_pWidth * pBMPInfo->header.bits + 7) / 8 * abs( *_pHeight );
	}

	// Allocate space for the bitmap itself
	free( pBMPInfo );
	pBits = (GLbyte*)malloc( sizeof( GLbyte) * bitSize );

	// Read and check for corruption
	if( 1 != fread( pBits, bitSize, 1, pFile ) )
	{
		free( pBits );
		pBits = NULL;
	}

	fclose( pFile );

	return pBits;
}

// Load a .PNG file
//void glsReadPNGBits( const char* _fileName, GLint* _width, GLint* _height /* params */ )
//{

//}

// Load a .TGA file
#pragma pack(1)
typedef struct
{
	GLbyte	identsize;				// Size of ID field that follows header (0)
	GLbyte	colorMapType;			// 0 = None, 1 = paletted
	GLbyte	imageType;				// 0 = none, 1 = indexed, 2 = rgb, 3 = grey, +8=rle
	unsigned short	colorMapStart;	// First colour map entry
	unsigned short	colorMapLength;	// Number of colors
	unsigned char 	colorMapBits;	// bits per palette entry
	unsigned short	xstart;			// image x origin
	unsigned short	ystart;			// image y origin
	unsigned short	width;			// width in pixels
	unsigned short	height;			// height in pixels
	GLbyte	bits;					// bits per pixel (8 16, 24, 32)
	GLbyte	descriptor;				// image descriptor
} TGAHEADER;
#pragma pack(8)

GLbyte *glsReadTGABits( const char* _fileName, GLint* _width , GLint* _height, GLint* _components, GLenum* _format )
{
	FILE *pFile;				// File pointer
	TGAHEADER tgaHeader;		// TGA file header
	unsigned long lImageSize;	// Size in bytes of image
	short sDepth;				// Pixel depth;
	GLbyte	*pBits = nullptr;	// Pointer to bits

	// Default/Failed values
	*_width = 0;
	*_height= 0;
	*_format = GL_RGB;
	*_components = GL_RGB;

	// Attempt to open the file
	//pFile = fopen( _fileName, "rb" );
	fopen_s( &pFile, _fileName, "rb" );
	if( pFile == nullptr )
		return 0;

	// Read in header (binary)
	fread(&tgaHeader, 18/* sizeof(TGAHEADER)*/, 1, pFile);

	// Get width, height, and depth of texture
	*_width = tgaHeader.width;
	*_height = tgaHeader.height;
	sDepth = tgaHeader.bits / 8;

	// Put some validity checks here. Very simply, I only understand
	// or care about 8, 24, or 32 bit targa's.
	if(tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
		return NULL;

	// Calculate size of image buffer
	lImageSize = tgaHeader.width * tgaHeader.height * sDepth;

	// Allocate memory and check for success
	pBits = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));
	if(pBits == NULL)
		return NULL;

	// Read in the bits
	// Check for read error. This should catch RLE or other 
	// weird formats that I don't want to recognize
	if(fread(pBits, lImageSize, 1, pFile) != 1)
	{
		free(pBits);
		return NULL;
	}

	// Set OpenGL format expected
	switch(sDepth)
	{
#ifndef OPENGL_ES
	case 3:     // Most likely case
		*_format = GL_BGR;
		*_components = GL_RGB;
		break;
#endif
	case 4:
		*_format = GL_BGRA;
		*_components = GL_RGBA;
		break;
	case 1:
		*_format = GL_LUMINANCE;
		*_components = GL_LUMINANCE;
		break;
	default:        // RGB
		// If on the iPhone, TGA's are BGR, and the iPhone does not 
		// support BGR without alpha, but it does support RGB,
		// so a simple swizzle of the red and blue bytes will suffice.
		// For faster iPhone loads however, save your TGA's with an Alpha!
#ifdef OPENGL_ES
		for(int i = 0; i < lImageSize; i+=3)
		{
			GLbyte temp = pBits[i];
			pBits[i] = pBits[i+2];
			pBits[i+2] = temp;
		}
#endif
		break;
	}



	// Done with File
	fclose(pFile);

	// Return pointer to image data
	return pBits;
}

// Create debug render objects

void glsCreateSphere( GLTriangleBatch& _sphereBatch, GLfloat _radius )
{
	GLfloat dRho = (GLfloat)GLS_PI / (GLfloat)8.0;
	GLfloat dTheta = 2.0f * (GLfloat)GLS_PI / (GLfloat)16.0;
	GLfloat dU = 1.0f / (GLfloat)16.0;
	GLfloat dV = 1.0f / (GLfloat)8.0;
	GLfloat v = 1.0f;
	GLfloat u = 0.0f;
	GLint i, j;

	_sphereBatch.BeginMesh( 768, 768 );
	for( i = 0; i < 8; ++i )
	{
		GLfloat rho = (GLfloat)i * dRho;
		GLfloat sRho = (GLfloat)(sin( rho ));
		GLfloat cRho = (GLfloat)(cos( rho ));
		GLfloat sRhoDRho = (GLfloat)(sin( rho + dRho ));
		GLfloat cRhoDRho = (GLfloat)(cos( rho + dRho ));

		u = 0.0f;
		Vec3f vVertex[4];
		Vec3f vNormal[4];
		Vec2f vTexture[4];

		for( j = 0; j < 16; ++j )
		{
			GLfloat theta = (j == 16) ? 0.0f : j * dTheta;
			GLfloat sTheta = (GLfloat)(-sin( theta ));
			GLfloat cTheta = (GLfloat)(cos( theta ));

			GLfloat x = sTheta * sRho;
			GLfloat y = cTheta * sRho;
			GLfloat z = cRho;

			vTexture[0][0] = u;
			vTexture[0][1] = v;
			vNormal[0][0] = x;
			vNormal[0][1] = y;
			vNormal[0][2] = z;
			vVertex[0][0] = x * _radius;
			vVertex[0][1] = y * _radius;
			vVertex[0][2] = z * _radius;

			x = sTheta * sRhoDRho;
			y = cTheta * sRhoDRho;
			z = cRhoDRho;

			vTexture[1][0] = u;
			vTexture[1][1] = v - dV;
			vNormal[1][0] = x;
			vNormal[1][1] = y;
			vNormal[1][2] = z;
			vVertex[1][0] = x * _radius;
			vVertex[1][1] = y * _radius;
			vVertex[1][2] = z * _radius;

			theta = ((j + 1) == 16) ? 0.0f : (j + 1) * dTheta;
			sTheta = (GLfloat)(-sin( theta ));
			cTheta = (GLfloat)(cos( theta ));

			x = sTheta * sRho;
			y = cTheta * sRho;
			z = cRho;

			u += dU;
			vTexture[2][0] = u;
			vTexture[2][1] = v;
			vNormal[2][0] = x;
			vNormal[2][1] = y;
			vNormal[2][2] = z;
			vVertex[2][0] = x * _radius;
			vVertex[2][1] = y * _radius;
			vVertex[2][2] = z * _radius;

			x = sTheta * sRhoDRho;
			y = cTheta * sRhoDRho;
			z = cRhoDRho;

			vTexture[3][0] = u;
			vTexture[3][1] = v - dV;
			vNormal[3][0] = x;
			vNormal[3][1] = y;
			vNormal[3][2] = z;
			vVertex[3][0] = x * _radius;
			vVertex[3][1] = y * _radius;
			vVertex[3][2] = z * _radius;

			_sphereBatch.AddTriangle( vVertex, vNormal, vTexture );

			// Get set up for the next triangle
			memcpy( vVertex[0], vVertex[1], sizeof( Vec3f ));
			memcpy( vNormal[0], vNormal[1], sizeof( Vec3f ));
			memcpy( vTexture[0], vTexture[1], sizeof( Vec2f ));

			memcpy( vVertex[1], vVertex[3], sizeof( Vec3f ));
			memcpy( vNormal[1], vNormal[3], sizeof( Vec3f ));
			memcpy( vTexture[1], vTexture[3], sizeof( Vec2f ));

			_sphereBatch.AddTriangle( vVertex, vNormal, vTexture );
		}
		v -= dV;
	}
	_sphereBatch.End();
}

void glsCreateCube( GLBatch& _cubeBatch, GLfloat _radius )
{
	_cubeBatch.Begin( GL_TRIANGLES, 36, 1 );

	// top
	_cubeBatch.Normal3f( 0.0f, _radius, 0.0f );
	_cubeBatch.MultiTexCoord2f( 0, _radius, _radius );
	_cubeBatch.Vertex3f( _radius, _radius, _radius );

	_cubeBatch.Normal3f( 0.0f, _radius, 0.0f );
	_cubeBatch.MultiTexCoord2f( 0, _radius, 0.0f );
	_cubeBatch.Vertex3f( _radius, _radius, -_radius );

	_cubeBatch.Normal3f( 0.0f, _radius, 0.0f );
	_cubeBatch.MultiTexCoord2f( 0, 0.0f, 0.0f );
	_cubeBatch.Vertex3f( -_radius, _radius, -_radius );

	_cubeBatch.Normal3f( 0.0f, _radius, 0.0f );
	_cubeBatch.MultiTexCoord2f( 0, _radius, _radius );
	_cubeBatch.Vertex3f( _radius, _radius, _radius );

	_cubeBatch.Normal3f( 0.0f, _radius, 0.0f );
	_cubeBatch.MultiTexCoord2f( 0, 0.0f, 0.0f );
	_cubeBatch.Vertex3f( -_radius, _radius, -_radius );

	_cubeBatch.Normal3f( 0.0f, _radius, 0.0f );
	_cubeBatch.MultiTexCoord2f( 0, 0.0f, _radius );
	_cubeBatch.Vertex3f( -_radius, _radius, _radius );

	// bottom
	_cubeBatch.Normal3f( 0.0f, -_radius, 0.0f );
	_cubeBatch.MultiTexCoord2f( 0, 0.0f, 0.0f );
	_cubeBatch.Vertex3f( -_radius, -_radius, -_radius );

	_cubeBatch.Normal3f( 0.0f, -_radius, 0.0f );
	_cubeBatch.MultiTexCoord2f( 0, _radius, 0.0f );
	_cubeBatch.Vertex3f( _radius, -_radius, -_radius );

	_cubeBatch.Normal3f( 0.0f, -_radius, 0.0f );
	_cubeBatch.MultiTexCoord2f( 0, _radius, _radius );
	_cubeBatch.Vertex3f( _radius, -_radius, _radius );

	_cubeBatch.Normal3f( 0.0f, -_radius, 0.0f );
	_cubeBatch.MultiTexCoord2f( 0, 0.0f, _radius );
	_cubeBatch.Vertex3f( -_radius, -_radius, _radius );

	_cubeBatch.Normal3f( 0.0f, -_radius, 0.0f );
	_cubeBatch.MultiTexCoord2f( 0, 0.0f, 0.0f );
	_cubeBatch.Vertex3f( -_radius, -_radius, -_radius );

	_cubeBatch.Normal3f( 0.0f, -_radius, 0.0f );
	_cubeBatch.MultiTexCoord2f( 0, _radius, _radius );
	_cubeBatch.Vertex3f( _radius, -_radius, _radius );

	// left
	_cubeBatch.Normal3f( -_radius, 0.0f, 0.0f );
	_cubeBatch.MultiTexCoord2f( 0, _radius, _radius );
	_cubeBatch.Vertex3f( -_radius, _radius, _radius );

	_cubeBatch.Normal3f( -_radius, 0.0f, 0.0f );
	_cubeBatch.MultiTexCoord2f( 0, _radius, 0.0f );
	_cubeBatch.Vertex3f( -_radius, _radius, -_radius );

	_cubeBatch.Normal3f( -_radius, 0.0f, 0.0f );
	_cubeBatch.MultiTexCoord2f( 0, 0.0f, 0.0f );
	_cubeBatch.Vertex3f( -_radius, -_radius, -_radius );

	_cubeBatch.Normal3f( -_radius, 0.0f, 0.0f );
	_cubeBatch.MultiTexCoord2f( 0, _radius, _radius );
	_cubeBatch.Vertex3f( -_radius, _radius, _radius );

	_cubeBatch.Normal3f( -_radius, 0.0f, 0.0f );
	_cubeBatch.MultiTexCoord2f( 0, 0.0f, 0.0f );
	_cubeBatch.Vertex3f( -_radius, -_radius, -_radius );

	_cubeBatch.Normal3f( -_radius, 0.0f, 0.0f );
	_cubeBatch.MultiTexCoord2f( 0, 0.0f, _radius );
	_cubeBatch.Vertex3f( -_radius, -_radius, _radius );

	// right
	_cubeBatch.Normal3f( _radius, 0.0f, 0.0f );
	_cubeBatch.MultiTexCoord2f( 0, 0.0f, 0.0f );
	_cubeBatch.Vertex3f( _radius, -_radius, -_radius );

	_cubeBatch.Normal3f( _radius, 0.0f, 0.0f );
	_cubeBatch.MultiTexCoord2f( 0, _radius, 0.0f );
	_cubeBatch.Vertex3f( _radius, _radius, -_radius );

	_cubeBatch.Normal3f( _radius, 0.0f, 0.0f );
	_cubeBatch.MultiTexCoord2f( 0, _radius, _radius );
	_cubeBatch.Vertex3f( _radius, _radius, _radius );

	_cubeBatch.Normal3f( _radius, 0.0f, 0.0f );
	_cubeBatch.MultiTexCoord2f( 0, _radius, _radius );
	_cubeBatch.Vertex3f( _radius, _radius, _radius );

	_cubeBatch.Normal3f( _radius, 0.0f, 0.0f );
	_cubeBatch.MultiTexCoord2f( 0, 0.0f, _radius );
	_cubeBatch.Vertex3f( _radius, -_radius, _radius );

	_cubeBatch.Normal3f( _radius, 0.0f, 0.0f );
	_cubeBatch.MultiTexCoord2f( 0, 0.0f, 0.0f );
	_cubeBatch.Vertex3f( _radius, -_radius, -_radius );

	// front
	_cubeBatch.Normal3f( 0.0f, 0.0f, _radius );
	_cubeBatch.MultiTexCoord2f( 0, _radius, 0.0f );
	_cubeBatch.Vertex3f( _radius, -_radius, _radius );

	_cubeBatch.Normal3f( 0.0f, 0.0f, _radius );
	_cubeBatch.MultiTexCoord2f( 0, _radius, _radius );
	_cubeBatch.Vertex3f( _radius, _radius, _radius );

	_cubeBatch.Normal3f( 0.0f, 0.0f, _radius );
	_cubeBatch.MultiTexCoord2f( 0, 0.0f, _radius );
	_cubeBatch.Vertex3f( -_radius, _radius, _radius );

	_cubeBatch.Normal3f( 0.0f, 0.0f, _radius );
	_cubeBatch.MultiTexCoord2f( 0, 0.0f, _radius );
	_cubeBatch.Vertex3f( -_radius, _radius, _radius );

	_cubeBatch.Normal3f( 0.0f, 0.0f, _radius );
	_cubeBatch.MultiTexCoord2f( 0, 0.0f, 0.0f );
	_cubeBatch.Vertex3f( -_radius, -_radius, _radius );

	_cubeBatch.Normal3f( 0.0f, 0.0f, _radius );
	_cubeBatch.MultiTexCoord2f( 0, _radius, 0.0f );
	_cubeBatch.Vertex3f( _radius, -_radius, _radius );

	// back
	_cubeBatch.Normal3f( 0.0f, 0.0f, -_radius );
	_cubeBatch.MultiTexCoord2f( 0, _radius, 0.0f );
	_cubeBatch.Vertex3f( _radius, -_radius, -_radius );

	_cubeBatch.Normal3f( 0.0f, 0.0f, -_radius );
	_cubeBatch.MultiTexCoord2f( 0, 0.0f, 0.0f );
	_cubeBatch.Vertex3f( -_radius, -_radius, -_radius );

	_cubeBatch.Normal3f( 0.0f, 0.0f, -_radius );
	_cubeBatch.MultiTexCoord2f( 0, 0.0f, _radius );
	_cubeBatch.Vertex3f( -_radius, _radius, -_radius );

	_cubeBatch.Normal3f( 0.0f, 0.0f, -_radius );
	_cubeBatch.MultiTexCoord2f( 0, 0.0f, _radius );
	_cubeBatch.Vertex3f( -_radius, _radius, -_radius );

	_cubeBatch.Normal3f( 0.0f, 0.0f, -_radius );
	_cubeBatch.MultiTexCoord2f( 0, _radius, _radius );
	_cubeBatch.Vertex3f( _radius, _radius, -_radius );

	_cubeBatch.Normal3f( 0.0f, 0.0f, -_radius );
	_cubeBatch.MultiTexCoord2f( 0, _radius, 0.0f );
	_cubeBatch.Vertex3f( _radius, -_radius, -_radius );

	_cubeBatch.End();
}

// Load the shader from the source text
void glsLoadShaderSrc( const char* _src, GLuint _shader )
{
	GLchar *fsStringPtr[1];

	fsStringPtr[0] = (GLchar *)_src;
	glShaderSource( _shader, 1, (const GLchar **)fsStringPtr, nullptr );
}

// Load a shader file
static GLubyte shaderText[MAX_SHADER_LENGTH];
bool glsLoadShaderFile( const char *_fileName, GLuint _shader )
{
	GLint shaderLength = 0;
	FILE *fp;

	// Open the _shader file
	fopen_s(&fp, _fileName, "r");
	if(fp != NULL)
	{
		// See how long the file is
		while (fgetc(fp) != EOF)
			shaderLength++;

		// Allocate a block of memory to send in the _shader
		assert(shaderLength < MAX_SHADER_LENGTH);   // make me bigger!
		if(shaderLength > MAX_SHADER_LENGTH)
		{
			fclose(fp);
			return false;
		}

		// Go back to beginning of file
		rewind(fp);

		// Read the whole file in
		if (shaderText != NULL)
			fread(shaderText, 1, shaderLength, fp);

		// Make sure it is null terminated and close the file
		shaderText[shaderLength] = '\0';
		fclose(fp);
	}
	else
		return false;    

	// Load the string
	glsLoadShaderSrc((const char *)shaderText, _shader);

	return true;
}   

GLuint glsLoadShaderPairWithAttributes( const char *_vertexShader, const char *_pixelShader, ... )
{
	// Temporary Shader objects
	GLuint hVertexShader;
	GLuint hFragmentShader; 
	GLuint hReturn = 0;   
	GLint testVal;

	// Create shader objects
	hVertexShader = glCreateShader(GL_VERTEX_SHADER);
	hFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Load them. If fail clean up and return null
	// Vertex Program
	if(glsLoadShaderFile(_vertexShader, hVertexShader) == false)
	{
		glDeleteShader(hVertexShader);
		glDeleteShader(hFragmentShader);
		//fprintf(stderr, "The shader at %s could ot be found.\n", _vertexShader);
		std::cout << "The shader at " << _vertexShader << " could not be found.\n";
		return (GLuint)NULL;
	}

	// Fragment Program
	if(glsLoadShaderFile(_pixelShader, hFragmentShader) == false)
	{
		glDeleteShader(hVertexShader);
		glDeleteShader(hFragmentShader);
		//fprintf(stderr,"The shader at %s  could not be found.\n", _pixelShader);
		std::cout << "The shader at " << _pixelShader << " could not be found.\n";
		return (GLuint)NULL;
	}

	// Compile them both
	glCompileShader(hVertexShader);
	glCompileShader(hFragmentShader);

	// Check for errors in vertex shader
	glGetShaderiv(hVertexShader, GL_COMPILE_STATUS, &testVal);
	if(testVal == GL_FALSE)
	{
		char infoLog[1024];
		glGetShaderInfoLog(hVertexShader, 1024, NULL, infoLog);
		//fprintf(stderr, "The shader at %s failed to compile with the following error:\n%s\n", _vertexShader, infoLog);
		std::cout << "The shader at " << _vertexShader << " failed to compile with the following error: " << infoLog << std::endl;
		glDeleteShader(hVertexShader);
		glDeleteShader(hFragmentShader);
		return (GLuint)NULL;
	}

	// Check for errors in fragment shader
	glGetShaderiv(hFragmentShader, GL_COMPILE_STATUS, &testVal);
	if(testVal == GL_FALSE)
	{
		char infoLog[1024];
		glGetShaderInfoLog(hFragmentShader, 1024, NULL, infoLog);
		//fprintf(stderr, "The shader at %s failed to compile with the following error:\n%s\n", _pixelShader, infoLog);
		std::cout << "The shader at " << _pixelShader << " failed to compile with the following error: " << infoLog << std::endl;
		glDeleteShader(hVertexShader);
		glDeleteShader(hFragmentShader);
		return (GLuint)NULL;
	}

	// Create the final program object, and attach the shaders
	hReturn = glCreateProgram();
	glAttachShader(hReturn, hVertexShader);
	glAttachShader(hReturn, hFragmentShader);


	// Now, we need to bind the attribute names to their specific locations
	// List of attributes
	va_list attributeList;
	va_start(attributeList, _pixelShader);

	// Iterate over this argument list
	char *szNextArg;
	int iArgCount = va_arg(attributeList, int);	// Number of attributes
	for(int i = 0; i < iArgCount; i++)
	{
		int index = va_arg(attributeList, int);
		szNextArg = va_arg(attributeList, char*);
		glBindAttribLocation(hReturn, index, szNextArg);
	}
	va_end(attributeList);

	// Attempt to link    
	glLinkProgram(hReturn);

	// These are no longer needed
	glDeleteShader(hVertexShader);
	glDeleteShader(hFragmentShader);  

	// Make sure link worked too
	glGetProgramiv(hReturn, GL_LINK_STATUS, &testVal);
	if(testVal == GL_FALSE)
	{
		char infoLog[1024];
		glGetProgramInfoLog(hReturn, 1024, NULL, infoLog);
		//fprintf(stderr,"The programs %s and %s failed to link with the following errors:\n%s\n",
		//	_vertexShader, _pixelShader, infoLog);
		std::cout << "The shaders " << _vertexShader << " and " << _pixelShader <<
			" failed to link with the following errors: " << infoLog << std::endl;
		glDeleteProgram(hReturn);
		return (GLuint)NULL;
	}

	// All done, return our ready to use shader program
	return hReturn;  
}

/////////////////////////////////////////////////////////////////
// Load a pair of shaders, compile, and link together. Specify the complete
// file path for each shader. Note, there is no support for
// just loading say a vertex program... you have to do both.
GLuint glsLoadShaderPair( const char* _vertexShader, const char* _pixelShader )
{
	// Temporary Shader objects
	GLuint hVertexShader;
	GLuint hPixelShader; 
	GLuint hReturn = 0;   
	GLint testVal;

	// Create shader objects
	hVertexShader = glCreateShader(GL_VERTEX_SHADER);
	hPixelShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Load them. If fail clean up and return null
	if( glsLoadShaderFile( _vertexShader, hPixelShader ) == false )
	{
		glDeleteShader( hVertexShader );
		glDeleteShader( hPixelShader );
		return (GLuint)0;
	}

	if( glsLoadShaderFile( _pixelShader, hPixelShader ) == false )
	{
		glDeleteShader( hVertexShader );
		glDeleteShader( hPixelShader );
		return (GLuint)NULL;
	}

	// Compile them
	glCompileShader( hVertexShader );
	glCompileShader( hPixelShader );

	// Check for errors
	glGetShaderiv( hVertexShader, GL_COMPILE_STATUS, &testVal );
	if( testVal == GL_FALSE )
	{
		glDeleteShader( hVertexShader );
		glDeleteShader( hPixelShader );
		return (GLuint)0;
	}

	glGetShaderiv( hPixelShader, GL_COMPILE_STATUS, &testVal );
	if(testVal == GL_FALSE)
	{
		glDeleteShader( hVertexShader );
		glDeleteShader( hPixelShader );
		return (GLuint)0;
	}

	hReturn = glCreateProgram();
	glAttachShader( hReturn, hVertexShader );
	glAttachShader( hReturn, hPixelShader );

	glLinkProgram(hReturn);

	// These are no longer needed
	glDeleteShader( hVertexShader );
	glDeleteShader( hPixelShader );  

	// Make sure link worked too
	glGetProgramiv( hReturn, GL_LINK_STATUS, &testVal );
	if( testVal == GL_FALSE )
	{
		glDeleteProgram( hReturn );
		return (GLuint)0;
	}

	return hReturn;  
}   

/////////////////////////////////////////////////////////////////
// Load a pair of shaders, compile, and link together. Specify the complete
// file path for each shader. Note, there is no support for
// just loading say a vertex program... you have to do both.
GLuint glsLoadShaderPairSrc(const char *szVertexSrc, const char *szFragmentSrc)
{
	// Temporary Shader objects
	GLuint hVertexShader;
	GLuint hFragmentShader; 
	GLuint hReturn = 0;   
	GLint testVal;

	// Create shader objects
	hVertexShader = glCreateShader(GL_VERTEX_SHADER);
	hFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Load them. 
	glsLoadShaderSrc(szVertexSrc, hVertexShader);
	glsLoadShaderSrc(szFragmentSrc, hFragmentShader);

	// Compile them
	glCompileShader(hVertexShader);
	glCompileShader(hFragmentShader);

	// Check for errors
	glGetShaderiv(hVertexShader, GL_COMPILE_STATUS, &testVal);
	if(testVal == GL_FALSE)
	{
		glDeleteShader(hVertexShader);
		glDeleteShader(hFragmentShader);
		return (GLuint)NULL;
	}

	glGetShaderiv(hFragmentShader, GL_COMPILE_STATUS, &testVal);
	if(testVal == GL_FALSE)
	{
		glDeleteShader(hVertexShader);
		glDeleteShader(hFragmentShader);
		return (GLuint)NULL;
	}

	// Link them - assuming it works...
	hReturn = glCreateProgram();
	glAttachShader(hReturn, hVertexShader);
	glAttachShader(hReturn, hFragmentShader);
	glLinkProgram(hReturn);

	// These are no longer needed
	glDeleteShader(hVertexShader);
	glDeleteShader(hFragmentShader);  

	// Make sure link worked too
	glGetProgramiv(hReturn, GL_LINK_STATUS, &testVal);
	if(testVal == GL_FALSE)
	{
		glDeleteProgram(hReturn);
		return (GLuint)NULL;
	}

	return hReturn;  
}   

/////////////////////////////////////////////////////////////////
// Load a pair of shaders, compile, and link together. Specify the complete
// source code text for each shader. Note, there is no support for
// just loading say a vertex program... you have to do both.
GLuint glsLoadShaderPairSrcWithAttributes(const char *szVertexSrc, const char *szFragmentSrc, ...)
{
	// Temporary Shader objects
	GLuint hVertexShader;
	GLuint hFragmentShader; 
	GLuint hReturn = 0;   
	GLint testVal;

	// Create shader objects
	hVertexShader = glCreateShader(GL_VERTEX_SHADER);
	hFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Load them. 
	glsLoadShaderSrc(szVertexSrc, hVertexShader);
	glsLoadShaderSrc(szFragmentSrc, hFragmentShader);

	// Compile them
	glCompileShader(hVertexShader);
	glCompileShader(hFragmentShader);

	// Check for errors
	glGetShaderiv(hVertexShader, GL_COMPILE_STATUS, &testVal);
	if(testVal == GL_FALSE)
	{
		glDeleteShader(hVertexShader);
		glDeleteShader(hFragmentShader);
		return (GLuint)NULL;
	}

	glGetShaderiv(hFragmentShader, GL_COMPILE_STATUS, &testVal);
	if(testVal == GL_FALSE)
	{
		glDeleteShader(hVertexShader);
		glDeleteShader(hFragmentShader);
		return (GLuint)NULL;
	}

	// Link them - assuming it works...
	hReturn = glCreateProgram();
	glAttachShader(hReturn, hVertexShader);
	glAttachShader(hReturn, hFragmentShader);

	// List of attributes
	va_list attributeList;
	va_start(attributeList, szFragmentSrc);

	char *szNextArg;
	int iArgCount = va_arg(attributeList, int);	// Number of attributes
	for(int i = 0; i < iArgCount; i++)
	{
		int index = va_arg(attributeList, int);
		szNextArg = va_arg(attributeList, char*);
		glBindAttribLocation(hReturn, index, szNextArg);
	}
	va_end(attributeList);


	glLinkProgram(hReturn);

	// These are no longer needed
	glDeleteShader(hVertexShader);
	glDeleteShader(hFragmentShader);  

	// Make sure link worked too
	glGetProgramiv(hReturn, GL_LINK_STATUS, &testVal);
	if(testVal == GL_FALSE)
	{
		glDeleteProgram(hReturn);
		return (GLuint)NULL;
	}

	return hReturn;  
}   

// Check for Errors
bool glsCheckShaderErrors( GLuint _shaderName )
{
	bool foundError = false;
	GLenum error = glGetError();

	if( error != GL_NO_ERROR )
	{
	//	MessageBoxA( nullptr, "OpenGL Error", 0, 0 );
		foundError = true;
	}

	GLenum fboStatus = glCheckFramebufferStatus( GL_DRAW_FRAMEBUFFER );

	if( fboStatus != GL_FRAMEBUFFER_COMPLETE )
	{
		foundError = true;
		MessageBoxA( 0, "The framebuffer is not complete.", 0, 0 );

		switch( fboStatus )
		{
		case GL_FRAMEBUFFER_UNDEFINED:
			MessageBoxA( 0, "GL_FRAMEBUFFER_UNDEFINED", 0, 0 );
			break;
			
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			MessageBoxA( 0, "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT", 0, 0 );
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			MessageBoxA( 0, "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER", 0, 0 );
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			MessageBoxA( 0, "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER", 0, 0 );
			break;

		case GL_FRAMEBUFFER_UNSUPPORTED:
			MessageBoxA( 0, "GL_FRAMEBUFFER_UNSUPPORTED", 0, 0 );
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			MessageBoxA( 0, "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE", 0, 0 );
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
			MessageBoxA( 0, "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS", 0, 0 );
			break;
		}
	}

	if( _shaderName )
	{
		glValidateProgram( _shaderName );
		int isProgValid = 0;
		glGetProgramiv( _shaderName, GL_VALIDATE_STATUS, &isProgValid );
		if( !isProgValid )
		{
			foundError = true;

			char szName[10];
			_itoa_s( _shaderName, szName, 10 );

			MessageBoxA( 0, "Shader is not valid.", szName, 0 );
		}
	}
	return foundError;
}

// Create a matrix that maps geometry to the screen
// 1 unit = 1px on the x and/or y axes
void glsCreateOrtho2DMatrix( GLuint _screenWidth, GLuint _screenHeight, Matrix4x4f& orthoMatrix, GLBatch& screenQuad )
{
	float right = (float)_screenWidth;
	float left  = 0.0f;
	float top = (float)_screenHeight;
	float bottom = 0.0f;

	// set ortho matrix
	orthoMatrix[0] = (float)(2 / (right - left));
	orthoMatrix[1] = 0.0;
	orthoMatrix[2] = 0.0;
	orthoMatrix[3] = 0.0;
	
	orthoMatrix[4] = 0.0;
	orthoMatrix[5] = (float)(2 / (top - bottom));
	orthoMatrix[6] = 0.0;
	orthoMatrix[7] = 0.0;
	
	orthoMatrix[8] = 0.0;
	orthoMatrix[9] = 0.0;
	orthoMatrix[10] = (float)(-2 / (1.0 - 0.0));
	orthoMatrix[11] = 0.0;
	
	orthoMatrix[12] = -1 * (right + left) / (right - left);
	orthoMatrix[13] = -1 * (top + bottom) / (top - bottom);
	orthoMatrix[14] = -1.0f;
	orthoMatrix[15] =  1.0;

	// set screen quad vertex array
	screenQuad.Reset();

	screenQuad.Begin( GL_TRIANGLE_STRIP, 4, 1 );

	screenQuad.Color4f( 0.0f, 1.0f, 0.0f, 1.0f );
	screenQuad.MultiTexCoord2f( 0, 0.0f, 0.0f ); 
	screenQuad.Vertex3f( 0.0f, 0.0f, 0.0f );

	screenQuad.Color4f( 0.0f, 1.0f, 0.0f, 1.0f );
	screenQuad.MultiTexCoord2f( 0, 1.0f, 0.0f );
	screenQuad.Vertex3f( (float)_screenWidth, 0.0f, 0.0f );

	screenQuad.Color4f( 0.0f, 1.0f, 0.0f, 1.0f );
	screenQuad.MultiTexCoord2f( 0, 0.0f, 1.0f );
	screenQuad.Vertex3f( 0.0f, (float)_screenHeight, 0.0f );

	screenQuad.Color4f( 0.0f, 1.0f, 0.0f, 1.0f );
	screenQuad.MultiTexCoord2f( 0, 1.0f, 1.0f );
	screenQuad.Vertex3f( (float)_screenWidth, (float)_screenHeight, 0.0f );

	screenQuad.End();
}


bool _glsCheckGLErrors( void )
	{
	GLuint result = glGetError();
	bool errorFound = false;

	while( result != GL_NO_ERROR )
	{
		errorFound = true;

		switch( result )
		{
		case GL_INVALID_ENUM:
			std::cout << "GL_INVALID_ENUM\n";
			break;
		case GL_INVALID_VALUE:
			std::cout << "GL_INVALID_VALUE\n";
			break;
		case GL_INVALID_OPERATION:
			std::cout << "GL_INVALID_OPERATION\n";
			break;
		case GL_STACK_OVERFLOW:
			std::cout << "GL_STACK_OVERFLOW\n";
			break;
		case GL_STACK_UNDERFLOW:
			std::cout << "GL_STACK_UNDERFLOW\n";
			break;
		case GL_OUT_OF_MEMORY:
			std::cout << "GL_OUT_OF_MEMORY\n";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION\n";
			break;
		case GL_TABLE_TOO_LARGE:
			std::cout << "GL_TABLE_TOO_LARGE\n";
			break;
		};

		result = glGetError();
	}

	return errorFound;
}