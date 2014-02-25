// Textured directional light vertex shader
#version 130

uniform mat4 mvpMatrix;
uniform mat3 normalMatrix;

attribute vec4 vVertex;
attribute vec3 vNormal;
attribute vec2 vTexCoord0;
// Normal mapping is blocked.  I need to be able to pass in the tangent
// part of the TBN as an attribute before I can move on.

varying vec2 vTex;

smooth out vec3 vVaryingNormal;

void main( void )
{
	vVaryingNormal = normalMatrix * vNormal;
	vTex = vTexCoord0;
	gl_Position = mvpMatrix * vVertex;
}
