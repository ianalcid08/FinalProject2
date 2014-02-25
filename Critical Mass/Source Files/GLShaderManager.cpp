/*
File:		GLShaderManager.cpp
Author:		Bob Pasekoff
Updated:	7/16/2013

*/

#include "../Header Files/stdafx.h"
#include "../Header Files/GLShaderManager.h"
#include "../Header Files/GLSetup.h"

// Stock shaders
static const char* identityShaderVS =
	"attribute vec4 vVertex;"
	"void main(void) "
	"{ gl_Position = vVertex; "
	"}";

static const char* idendityShaderPS =
	"uniform vec4 vColor;"
	"void main(void) "
	"{ gl_FragColor = vColor;"
	"}";


static const char* flatShaderVS =
	"uniform mat4 mvpMatrix;"
	"attribute vec4 vVertex;"
	"void main(void) "
	"{ gl_Position = mvpMatrix * vVertex; "
	"}";

static const char* flatShaderPS = 
	"uniform vec4 vColor;"
	"void main(void) "
	"{ gl_FragColor = vColor; "
	"}";


static const char* shadedVS =	
	"uniform mat4 mvpMatrix;"
	"attribute vec4 vColor;"
	"attribute vec4 vVertex;"
	"varying vec4 vFragColor;"
	"void main(void) {"
	"vFragColor = vColor; "
	" gl_Position = mvpMatrix * vVertex; "
	"}";

static const char* shadedPS =     
	"varying vec4 vFragColor; "
	"void main(void) { "
	" gl_FragColor = vFragColor; "
	"}";


static const char* defaultLightVS =
	"uniform mat4 mvMatrix;"
	"uniform mat4 pMatrix;"
	"varying vec4 vFragColor;"
	"attribute vec4 vVertex;"
	"attribute vec3 vNormal;"
	"uniform vec4 vColor;"
	"void main(void) { "
	" mat3 mNormalMatrix;"
	" mNormalMatrix[0] = mvMatrix[0].xyz;"
	" mNormalMatrix[1] = mvMatrix[1].xyz;"
	" mNormalMatrix[2] = mvMatrix[2].xyz;"
	" vec3 vNorm = normalize(mNormalMatrix * vNormal);"
	" vec3 vLightDir = vec3(0.0, 0.0, 1.0); "
	" float fDot = max(0.0, dot(vNorm, vLightDir)); "
	" vFragColor.rgb = vColor.rgb * fDot;"
	" vFragColor.a = vColor.a;"
	" mat4 mvpMatrix;"
	" mvpMatrix = pMatrix * mvMatrix;"
	" gl_Position = mvpMatrix * vVertex; "
	"}";

static const char* defaultLightPS =	
	"varying vec4 vFragColor; "
	"void main(void) { "
	" gl_FragColor = vFragColor; "
	"}";


static const char* pointLightDiffVS =
	"uniform mat4 mvMatrix;"
	"uniform mat4 pMatrix;"
	"uniform vec3 vLightPos;"
	"uniform vec4 vColor;"
	"attribute vec4 vVertex;"
	"attribute vec3 vNormal;"
	"varying vec4 vFragColor;"
	"void main(void) { "
	" mat3 mNormalMatrix;"
	" mNormalMatrix[0] = normalize(mvMatrix[0].xyz);"
	" mNormalMatrix[1] = normalize(mvMatrix[1].xyz);"
	" mNormalMatrix[2] = normalize(mvMatrix[2].xyz);"
	" vec3 vNorm = normalize(mNormalMatrix * vNormal);"
	" vec4 ecPosition;"
	" vec3 ecPosition3;"
	" ecPosition = mvMatrix * vVertex;"
	" ecPosition3 = ecPosition.xyz /ecPosition.w;"
	" vec3 vLightDir = normalize(vLightPos - ecPosition3);"
	" float fDot = max(0.0, dot(vNorm, vLightDir)); "
	" vFragColor.rgb = vColor.rgb * fDot;"
	" vFragColor.a = vColor.a;"
	" mat4 mvpMatrix;"
	" mvpMatrix = pMatrix * mvMatrix;"
	" gl_Position = mvpMatrix * vVertex; "
	"}";

static const char* pointLightDiffPS = 
	"varying vec4 vFragColor; "
	"void main(void) { "
	" gl_FragColor = vFragColor; "
	"}";


static const char* textureReplaceVS =
	"uniform mat4 mvpMatrix;"
	"attribute vec4 vVertex;"
	"attribute vec2 vTexCoord0;"
	"varying vec2 vTex;"
	"void main(void) "
	"{ vTex = vTexCoord0;" 
	" gl_Position = mvpMatrix * vVertex; "
	"}";

static const char* textureReplacePS = 
	"varying vec2 vTex;"
	"uniform sampler2D textureUnit0;"
	"void main(void) "
	"{ gl_FragColor = texture2D(textureUnit0, vTex); "
	"}";


static const char* textureRectReplaceVS =
	"uniform mat4 mvpMatrix;"
	"attribute vec4 vVertex;"
	"attribute vec2 vTexCoord0;"
	"varying vec2 vTex;"
	"void main(void) "
	"{ vTex = vTexCoord0;" 
	" gl_Position = mvpMatrix * vVertex; "
	"}";

static const char* textureRectReplacePS = 
	"varying vec2 vTex;"
	"uniform sampler2DRect textureUnit0;"
	"void main(void) "
	"{ gl_FragColor = texture2DRect(textureUnit0, vTex); "
	"}";


static const char* textureModulateVS =
	"uniform mat4 mvpMatrix;"
	"attribute vec4 vVertex;"
	"attribute vec2 vTexCoord0;"
	"varying vec2 vTex;"
	"void main(void) "
	"{ vTex = vTexCoord0;" 
	" gl_Position = mvpMatrix * vVertex; "
	"}";

static const char* textureModulatePS =
	"varying vec2 vTex;"
	"uniform sampler2D textureUnit0;"
	"uniform vec4 vColor;"
	"void main(void) "
	"{ gl_FragColor = vColor * texture2D(textureUnit0, vTex); "
	"}";


static const char* texturePointLightDiffVS =
	"uniform mat4 mvMatrix;"
	"uniform mat4 pMatrix;"
	"uniform vec3 vLightPos;"
	"uniform vec4 vColor;"
	"attribute vec4 vVertex;"
	"attribute vec3 vNormal;"
	"varying vec4 vFragColor;"
	"attribute vec2 vTexCoord0;"
	"varying vec2 vTex;"
	"void main(void) { "
	" mat3 mNormalMatrix;"
	" mNormalMatrix[0] = normalize(mvMatrix[0].xyz);"
	" mNormalMatrix[1] = normalize(mvMatrix[1].xyz);"
	" mNormalMatrix[2] = normalize(mvMatrix[2].xyz);"
	" vec3 vNorm = normalize(mNormalMatrix * vNormal);"
	" vec4 ecPosition;"
	" vec3 ecPosition3;"
	" ecPosition = mvMatrix * vVertex;"
	" ecPosition3 = ecPosition.xyz /ecPosition.w;"
	" vec3 vLightDir = normalize(vLightPos - ecPosition3);"
	" float fDot = max(0.0, dot(vNorm, vLightDir)); "
	" vFragColor.rgb = vColor.rgb * fDot;"
	" vFragColor.a = vColor.a;"
	" vTex = vTexCoord0;"
	" mat4 mvpMatrix;"
	" mvpMatrix = pMatrix * mvMatrix;"
	" gl_Position = mvpMatrix * vVertex; "
	"}";

static const char* texturePointLightDiffPS =	
	"varying vec4 vFragColor;"
	"varying vec2 vTex;"
	"uniform sampler2D textureUnit0;"
	"void main(void) { "
	" gl_FragColor = vFragColor * texture2D(textureUnit0, vTex);"
	"}";

static const char* UIShaderVS =
	"attribute vec4 vVertex;"
	"attribute vec2 vTexCoord0;"
	"varying vec2 vTex;"
	"void main(void) "
	"{ vTex = vTexCoord0;" 
	"gl_Position = vVertex; "
	"}";

static const char* UIShaderPS =
	"varying vec2 vTex;"
	"uniform sampler2DRect textureUnit0;"
	"uniform vec4 vColor;"
	"void main(void) "
	"{"
	"gl_FragColor = texture2DRect(textureUnit0, vTex) * vColor;"
	"}";


GLShaderManager::GLShaderManager( void )
{
	// Set stock shader handles to 0... uninitialized
	for( unsigned int i = 0; i < GLS_SHADER_LAST; ++i )
		stockShaders[i] = 0;
}

GLShaderManager::~GLShaderManager( void )
{
	ReleaseStockShaders();
}

void GLShaderManager::ReleaseStockShaders( void )
{
	glUseProgram( 0 );

	for( unsigned int i = 0; i < GLS_SHADER_LAST; ++i )
		glDeleteProgram( stockShaders[i] );
}

// Initialize and load the stock shaders
bool GLShaderManager::InitializeStockShaders( void )
{
	stockShaders[GLS_SHADER_IDENTITY] = glsLoadShaderPairSrcWithAttributes( identityShaderVS, idendityShaderPS, 1, GLS_ATTRIBUTE_VERTEX, "vVertex" );
	stockShaders[GLS_SHADER_FLAT] = glsLoadShaderPairSrcWithAttributes( flatShaderVS, flatShaderPS, 1, GLS_ATTRIBUTE_VERTEX, "vVertex" );
	stockShaders[GLS_SHADER_SHADED] = glsLoadShaderPairSrcWithAttributes( shadedVS, shadedPS, 2, GLS_ATTRIBUTE_VERTEX, "vVertex", GLS_ATTRIBUTE_COLOR, "vColor" );


	stockShaders[GLS_SHADER_DEFAULT_LIGHT]	= glsLoadShaderPairSrcWithAttributes( defaultLightVS, defaultLightPS, 2,
		GLS_ATTRIBUTE_VERTEX, "vVertex", GLS_ATTRIBUTE_NORMAL, "vNormal" );

	stockShaders[GLS_SHADER_POINT_LIGHT_DIFF] = glsLoadShaderPairSrcWithAttributes( pointLightDiffVS, pointLightDiffPS, 2,
		GLS_ATTRIBUTE_VERTEX, "vVertex", GLS_ATTRIBUTE_NORMAL, "vNormal" );

	stockShaders[GLS_SHADER_TEXTURE_REPLACE]  = glsLoadShaderPairSrcWithAttributes( textureReplaceVS, textureReplacePS, 2, 
		GLS_ATTRIBUTE_VERTEX, "vVertex", GLS_ATTRIBUTE_TEXTURE0, "vTexCoord0" );

	stockShaders[GLS_SHADER_TEXTURE_MODULATE] = glsLoadShaderPairSrcWithAttributes( textureModulateVS, textureModulatePS, 2,
		GLS_ATTRIBUTE_VERTEX, "vVertex", GLS_ATTRIBUTE_TEXTURE0, "vTexCoord0" );

	stockShaders[GLS_SHADER_TEXTURE_POINT_LIGHT_DIFF] = glsLoadShaderPairSrcWithAttributes( texturePointLightDiffVS, texturePointLightDiffPS, 3,
		GLS_ATTRIBUTE_VERTEX, "vVertex", GLS_ATTRIBUTE_NORMAL, "vNormal", GLS_ATTRIBUTE_TEXTURE0, "vTexCoord0" );


	stockShaders[GLS_SHADER_TEXTURE_RECT_REPLACE] = glsLoadShaderPairSrcWithAttributes( textureRectReplaceVS, textureRectReplacePS, 2, 
		GLS_ATTRIBUTE_VERTEX, "vVertex", GLS_ATTRIBUTE_TEXTURE0, "vTexCoord0" );

	stockShaders[GLS_SHADER_UI] = glsLoadShaderPairSrcWithAttributes( UIShaderVS, UIShaderPS, 2, 
		GLS_ATTRIBUTE_VERTEX, "vVertex", GLS_ATTRIBUTE_TEXTURE0, "vTexCoord0" );

	if( stockShaders[0] != 0 )
		return true;

	return false;
}


///////////////////////////////////////////////////////////////////////
// Use a specific stock shader, and set the appropriate uniforms
GLint GLShaderManager::UseStockShader( GLS_STOCK_SHADER _ShaderID, ... )
{
	// Check for out of bounds
	if( _ShaderID >= GLS_SHADER_LAST )
		return -1;

	// List of uniforms
	va_list uniformList;
	va_start( uniformList, _ShaderID );

	// Bind to the correct shader
	glUseProgram( stockShaders[_ShaderID] );

	// Set up the uniforms
	GLint iTransform, iModelMatrix, iProjMatrix, iColor, iLight, iTextureUnit;
	int				iInteger;
	Matrix4x4f* mvpMatrix;
	Matrix4x4f*  pMatrix;
	Matrix4x4f*  mvMatrix;
	Vec4f*  vColor;
	Vec3f*  vLightPos;

	switch( _ShaderID )
	{
	case GLS_SHADER_FLAT:			// Just the modelview projection matrix and the color
		iTransform = glGetUniformLocation( stockShaders[_ShaderID], "mvpMatrix" );
		mvpMatrix = va_arg( uniformList, Matrix4x4f* );
		glUniformMatrix4fv( iTransform, 1, GL_FALSE, *mvpMatrix );

		iColor = glGetUniformLocation( stockShaders[_ShaderID], "vColor" );
		vColor = va_arg( uniformList, Vec4f* );
		glUniform4fv( iColor, 1, *vColor );
		break;

	case GLS_SHADER_TEXTURE_RECT_REPLACE:
	case GLS_SHADER_TEXTURE_REPLACE:	// Just the texture place
		iTransform = glGetUniformLocation( stockShaders[_ShaderID], "mvpMatrix" );
		mvpMatrix = va_arg( uniformList, Matrix4x4f* );
		glUniformMatrix4fv( iTransform, 1, GL_FALSE, *mvpMatrix );

		iTextureUnit = glGetUniformLocation( stockShaders[_ShaderID], "textureUnit0" );
		iInteger = va_arg( uniformList, int );
		glUniform1i( iTextureUnit, iInteger );
		break;

	case GLS_SHADER_TEXTURE_MODULATE: // Multiply the texture by the geometry color
		iTransform = glGetUniformLocation( stockShaders[_ShaderID], "mvpMatrix" );
		mvpMatrix = va_arg( uniformList, Matrix4x4f* );
		glUniformMatrix4fv( iTransform, 1, GL_FALSE, *mvpMatrix );

		iColor = glGetUniformLocation( stockShaders[_ShaderID], "vColor" );
		vColor = va_arg( uniformList, Vec4f* );
		glUniform4fv( iColor, 1, *vColor );			

		iTextureUnit = glGetUniformLocation( stockShaders[_ShaderID], "textureUnit0" );
		iInteger = va_arg( uniformList, int );
		glUniform1i( iTextureUnit, iInteger );
		break;


	case GLS_SHADER_DEFAULT_LIGHT:
		iModelMatrix = glGetUniformLocation( stockShaders[_ShaderID], "mvMatrix" );
		mvMatrix = va_arg( uniformList, Matrix4x4f* );
		glUniformMatrix4fv( iModelMatrix, 1, GL_FALSE, *mvMatrix );

		iProjMatrix = glGetUniformLocation( stockShaders[_ShaderID], "pMatrix" );
		pMatrix = va_arg( uniformList, Matrix4x4f* );
		glUniformMatrix4fv( iProjMatrix, 1, GL_FALSE, *pMatrix );

		iColor = glGetUniformLocation( stockShaders[_ShaderID], "vColor" );
		vColor = va_arg( uniformList, Vec4f* );
		glUniform4fv( iColor, 1, *vColor );
		break;

	case GLS_SHADER_POINT_LIGHT_DIFF:
		iModelMatrix = glGetUniformLocation( stockShaders[_ShaderID], "mvMatrix" );
		mvMatrix = va_arg( uniformList, Matrix4x4f* );
		glUniformMatrix4fv( iModelMatrix, 1, GL_FALSE, *mvMatrix );

		iProjMatrix = glGetUniformLocation( stockShaders[_ShaderID], "pMatrix" );
		pMatrix = va_arg( uniformList, Matrix4x4f*);
		glUniformMatrix4fv( iProjMatrix, 1, GL_FALSE, *pMatrix );

		iLight = glGetUniformLocation( stockShaders[_ShaderID], "vLightPos" );
		vLightPos = va_arg( uniformList, Vec3f* );
		glUniform3fv( iLight, 1, *vLightPos );

		iColor = glGetUniformLocation( stockShaders[_ShaderID], "vColor" );
		vColor = va_arg( uniformList, Vec4f* );
		glUniform4fv( iColor, 1, *vColor );
		break;			

	case GLS_SHADER_TEXTURE_POINT_LIGHT_DIFF:
		iModelMatrix = glGetUniformLocation( stockShaders[_ShaderID], "mvMatrix" );
		mvMatrix = va_arg( uniformList, Matrix4x4f* );
		glUniformMatrix4fv( iModelMatrix, 1, GL_FALSE, *mvMatrix );

		iProjMatrix = glGetUniformLocation( stockShaders[_ShaderID], "pMatrix" );
		pMatrix = va_arg( uniformList, Matrix4x4f* );
		glUniformMatrix4fv( iProjMatrix, 1, GL_FALSE, *pMatrix );

		iLight = glGetUniformLocation( stockShaders[_ShaderID], "vLightPos" );
		vLightPos = va_arg( uniformList, Vec3f* );
		glUniform3fv( iLight, 1, *vLightPos );

		iColor = glGetUniformLocation( stockShaders[_ShaderID], "vColor" );
		vColor = va_arg( uniformList, Vec4f* );
		glUniform4fv( iColor, 1, *vColor );

		iTextureUnit = glGetUniformLocation( stockShaders[_ShaderID], "textureUnit0" );
		iInteger = va_arg( uniformList, int );
		glUniform1i( iTextureUnit, iInteger );
		break;


	case GLS_SHADER_SHADED:		// Just the modelview projection matrix. Color is an attribute
		iTransform = glGetUniformLocation( stockShaders[_ShaderID], "mvpMatrix");
		pMatrix = va_arg( uniformList, Matrix4x4f* );
		glUniformMatrix4fv( iTransform, 1, GL_FALSE, *pMatrix );
		break;

	case GLS_SHADER_IDENTITY:	// Just the Color
		iColor = glGetUniformLocation( stockShaders[_ShaderID], "vColor" );
		vColor = va_arg( uniformList, Vec4f* );
		glUniform4fv( iColor, 1, *vColor );

	case GLS_SHADER_UI:	// Just the texture please
		iTextureUnit = glGetUniformLocation( stockShaders[_ShaderID], "textureUnit0" );
		iInteger = va_arg( uniformList, int );
		glUniform1i( iTextureUnit, iInteger );
		iColor = glGetUniformLocation( stockShaders[_ShaderID], "vColor" );
		vColor = va_arg( uniformList, Vec4f * );
		glUniform4fv( iColor, 1, *vColor );
		break;
	default:
		break;
	}
	va_end( uniformList );

	return stockShaders[_ShaderID];
}

GLuint GLShaderManager::GetStockShader( GLS_STOCK_SHADER _ShaderID )
{
	if( _ShaderID >= GLS_SHADER_LAST )
		return 0;

	return stockShaders[_ShaderID];
}

GLuint GLShaderManager::LoadShaderPair( const char* _vertexShaderFileName, const char* _pixelShaderFileName )
{
	SHADERLOOKUPETRY shaderEntry;

	// Make sure it's not already loaded
	GLuint uiReturn = LookupShader( _vertexShaderFileName, _pixelShaderFileName );
	if( uiReturn != 0 )
		return uiReturn;

	// Load shader and test for fail
	shaderEntry.shaderID = glsLoadShaderPair( _vertexShaderFileName, _pixelShaderFileName );
	if(shaderEntry.shaderID == 0)
		return 0;

	// Add to the table
	strncpy_s( shaderEntry.vertexShaderName, _vertexShaderFileName, MAX_SHADER_NAME_LENGTH );
	strncpy_s( shaderEntry.pixelShaderName, _pixelShaderFileName, MAX_SHADER_NAME_LENGTH );
	//	shaderTable.push_back(shaderEntry);	
	return shaderEntry.shaderID;
}


GLuint GLShaderManager::LoadShaderPairSrc( const char* _name, const char* _vertexSrc, const char* _pixelSrc )
{
	// Just make it and return
	if( _name == NULL )
		return glsLoadShaderPairSrc( _vertexSrc, _pixelSrc );

	// It has a name, check for duplicate
	GLuint uiShader = LookupShader( _name, _name );
	if( uiShader != 0 )
		return uiShader;

	// Ok, make it and add to table
	SHADERLOOKUPETRY shaderEntry;
	shaderEntry.shaderID = glsLoadShaderPairSrc( _vertexSrc, _pixelSrc );
	if( shaderEntry.shaderID == 0 )
		return 0;	// Game over, won't compile

	// Add it...
	strncpy_s( shaderEntry.vertexShaderName, _name, MAX_SHADER_NAME_LENGTH );
	strncpy_s( shaderEntry.pixelShaderName, _name, MAX_SHADER_NAME_LENGTH );
	//	shaderTable.push_back(shaderEntry);	
	return shaderEntry.shaderID;		
}


// Load the shader file, with the supplied named attributes
GLuint GLShaderManager::LoadShaderPairWithAttributes( const char* _vertexShaderFileName, const char* _pixelShaderFileName, ... )
{
	// Check for duplicate
	GLuint uiShader = LookupShader( _vertexShaderFileName, _pixelShaderFileName );
	if( uiShader != 0 )
		return uiShader;

	SHADERLOOKUPETRY shaderEntry;

	// Temporary Shader objects
	GLuint hVertexShader;
	GLuint hPixelShader;   
	GLint testVal;

	// Create shader objects
	hVertexShader = glCreateShader(GL_VERTEX_SHADER);
	hPixelShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Load them. If fail clean up and return null
	if( glsLoadShaderFile( _vertexShaderFileName, hVertexShader ) == false )
	{
		glDeleteShader( hVertexShader );
		glDeleteShader( hPixelShader );
		return 0;
	}

	if( glsLoadShaderFile( _pixelShaderFileName, hPixelShader ) == false )
	{
		glDeleteShader( hVertexShader );
		glDeleteShader( hPixelShader );
		return 0;
	}

	// Compile them
	glCompileShader( hVertexShader );
	glCompileShader( hPixelShader );

	// Check for errors
	glGetShaderiv(hVertexShader, GL_COMPILE_STATUS, &testVal);
	if(testVal == GL_FALSE)
	{
		glDeleteShader( hVertexShader );
		glDeleteShader( hPixelShader );
		return 0;
	}

	glGetShaderiv( hPixelShader, GL_COMPILE_STATUS, &testVal );
	if( testVal == GL_FALSE )
	{
		glDeleteShader( hVertexShader );
		glDeleteShader( hPixelShader );
		return 0;
	}

	// Link them - assuming it works...
	shaderEntry.shaderID = glCreateProgram();
	glAttachShader( shaderEntry.shaderID, hVertexShader );
	glAttachShader( shaderEntry.shaderID, hPixelShader );


	// List of attributes
	va_list attributeList;
	va_start( attributeList, _pixelShaderFileName);

	char *szNextArg;
	int iArgCount = va_arg(attributeList, int);	// Number of attributes
	for(int i = 0; i < iArgCount; i++)
	{
		int index = va_arg(attributeList, int);
		szNextArg = va_arg(attributeList, char*);
		glBindAttribLocation(shaderEntry.shaderID, index, szNextArg);
	}

	va_end( attributeList );

	glLinkProgram( shaderEntry.shaderID );

	// These are no longer needed
	glDeleteShader( hVertexShader );
	glDeleteShader( hPixelShader );  

	// Make sure link worked too
	glGetProgramiv( shaderEntry.shaderID, GL_LINK_STATUS, &testVal );
	if( testVal == GL_FALSE )
	{
		glDeleteProgram( shaderEntry.shaderID );
		return 0;
	}


	// Add it...
	strncpy_s( shaderEntry.vertexShaderName, _vertexShaderFileName, MAX_SHADER_NAME_LENGTH );
	strncpy_s( shaderEntry.pixelShaderName, _pixelShaderFileName, MAX_SHADER_NAME_LENGTH );

	return shaderEntry.shaderID;		
}

GLuint GLShaderManager::LoadShaderPairSrcWithAttributes( const char* _name, const char* _vertexShader, const char* _pixelShader, ... )
{
	// Check for duplicate
	GLuint uiShader = LookupShader( _name, _name );
	if( uiShader != 0 )
		return uiShader;

	SHADERLOOKUPETRY shaderEntry;

	// Temporary Shader objects
	GLuint hVertexShader;
	GLuint hPixelShader;  
	GLint testVal;

	// Create shader objects
	hVertexShader = glCreateShader(GL_VERTEX_SHADER);
	hPixelShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Load them. 
	glsLoadShaderSrc( _vertexShader, hVertexShader );
	glsLoadShaderSrc( _pixelShader, hPixelShader );

	// Compile them
	glCompileShader( hVertexShader );
	glCompileShader( hPixelShader );

	// Check for errors
	glGetShaderiv( hVertexShader, GL_COMPILE_STATUS, &testVal );
	if( testVal == GL_FALSE )
	{
		glDeleteShader( hVertexShader );
		glDeleteShader( hPixelShader );
		return 0;
	}

	glGetShaderiv( hPixelShader, GL_COMPILE_STATUS, &testVal );
	if( testVal == GL_FALSE )
	{
		glDeleteShader( hVertexShader );
		glDeleteShader( hPixelShader );
		return 0;
	}

	// Link them - assuming it works...
	shaderEntry.shaderID = glCreateProgram();
	glAttachShader( shaderEntry.shaderID, hVertexShader );
	glAttachShader( shaderEntry.shaderID, hPixelShader );

	// List of attributes
	va_list attributeList;
	va_start( attributeList, _pixelShader );

	char *szNextArg;
	int iArgCount = va_arg( attributeList, int );	// Number of attributes
	for( int i = 0; i < iArgCount; i++ )
	{
		int index = va_arg( attributeList, int );
		szNextArg = va_arg( attributeList, char* );
		glBindAttribLocation( shaderEntry.shaderID, index, szNextArg );
	}
	va_end( attributeList );


	glLinkProgram( shaderEntry.shaderID );

	// These are no longer needed
	glDeleteShader( hVertexShader );
	glDeleteShader( hPixelShader );  

	// Make sure link worked too
	glGetProgramiv( shaderEntry.shaderID, GL_LINK_STATUS, &testVal );
	if( testVal == GL_FALSE )
	{
		glDeleteProgram( shaderEntry.shaderID );
		return 0;
	}

	// Add it...
	strncpy_s( shaderEntry.vertexShaderName, _name, MAX_SHADER_NAME_LENGTH );
	strncpy_s( shaderEntry.pixelShaderName, _name, MAX_SHADER_NAME_LENGTH );
	//	shaderTable.push_back(shaderEntry);	
	return shaderEntry.shaderID;		
}


GLuint GLShaderManager::LookupShader(const char *szVertexProg, const char *szFragProg)
{
	return 0;
}