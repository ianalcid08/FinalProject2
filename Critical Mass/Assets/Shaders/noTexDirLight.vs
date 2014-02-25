// Untextured directional light vertex shader
#version 130

uniform mat4 mvpMatrix;
uniform mat3 normalMatrix;

attribute vec4 vVertex;
attribute vec3 vNormal;

smooth out vec3 vVaryingNormal;

void main( void )
{
	vVaryingNormal = normalMatrix * vNormal;
	
	gl_Position = mvpMatrix * vVertex;
}