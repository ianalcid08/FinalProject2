/*
	File:		GLShaderManager.h
	Author:		Bob Pasekoff
	Updated:	7/16/2013

*/

#pragma once

#include "stdafx.h"
#include "GL\glew.h"

#define MAX_SHADER_NAME_LENGTH	64

enum GLS_STOCK_SHADER {
	GLS_SHADER_IDENTITY = 0, GLS_SHADER_FLAT, GLS_SHADER_SHADED,
	GLS_SHADER_DEFAULT_LIGHT, GLS_SHADER_POINT_LIGHT_DIFF, GLS_SHADER_TEXTURE_REPLACE,
	GLS_SHADER_TEXTURE_MODULATE, GLS_SHADER_TEXTURE_POINT_LIGHT_DIFF,
	GLS_SHADER_TEXTURE_RECT_REPLACE, GLS_SHADER_UI, GLS_SHADER_LAST
};

enum GLS_SHADER_ATTRIBUTE {
	GLS_ATTRIBUTE_VERTEX = 0, GLS_ATTRIBUTE_COLOR, GLS_ATTRIBUTE_NORMAL,
	GLS_ATTRIBUTE_TEXTURE0, GLS_ATTRIBUTE_TEXTURE1,	GLS_ATTRIBUTE_TEXTURE2,
	GLS_ATTRIBUTE_TEXTURE3, GLS_ATTRIBUTE_JINDICES, GLS_ATTRIBUTE_WEIGHTS, GLS_ATTRIBUTE_LAST
};

struct SHADERLOOKUPETRY {
	char vertexShaderName[MAX_SHADER_NAME_LENGTH];
	char pixelShaderName[MAX_SHADER_NAME_LENGTH];
	GLuint	shaderID;
};

class GLShaderManager
{
public:
	GLShaderManager( void );
	~GLShaderManager( void );

	bool InitializeStockShaders( void );
	void ReleaseStockShaders( void );

	// return stock shader handle
	GLuint GetStockShader( GLS_STOCK_SHADER shaderID );

	// stock shader, parameters
	GLint UseStockShader( GLS_STOCK_SHADER shaderID, ... );

	// Load a shader pair from file, return shader handle or NULL. 
	// Vertex shader name (minus file extension)
	// is saved in the lookup table
	GLuint LoadShaderPair( const char* vertexShaderFileName, const char* pixelShaderFileName );

	GLuint LoadShaderPairSrc( const char* name, const char *vertexSrc, const char* pixelSrc );

	GLuint LoadShaderPairWithAttributes( const char* vertexShaderFileName, const char* pixelShaderFileName, ... );
	GLuint LoadShaderPairSrcWithAttributes( const char* name, const char* vertexShader, const char* pixelShader, ... );

	// Lookup a previously loaded shader
	GLuint LookupShader( const char* vertexShader, const char* pixelShader = 0 );

protected:
	GLuint	stockShaders[GLS_SHADER_LAST];
};