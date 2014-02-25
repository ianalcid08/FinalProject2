#version 130

uniform mat4 mvpMatrix;
uniform mat4 scaleMatrix;
attribute vec4 vVertex;
attribute vec2 vTexCoord0;
varying vec2 vTex;

void main( void )
{
	vTex = vTexCoord0;
	vec4 pos =  scaleMatrix * vVertex;
	gl_Position = mvpMatrix * pos;
}