// Textured point light vertex shader
#version 130

uniform mat4 mvpMatrix;
uniform float elapsedTime;

attribute vec4 vVertex;
//attribute vec3 vNormal;
attribute vec2 vTexCoord0;

varying vec2 vTex;

void main( void )
{
	vTex = vTexCoord0;
	vTex[0] += elapsedTime * 0.1;

	gl_Position = mvpMatrix * vVertex;
}
