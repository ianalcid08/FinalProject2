// Screen-space quad vertex shader
#version 130

attribute vec4 vVertex;
attribute vec2 vTexCoord0;

varying vec2 vTex;

void main( void )
{
	gl_Position = vVertex;
	vTex = vTexCoord0;	
}