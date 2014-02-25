// Ripple effect vertex shader
#version 130

uniform mat4 mvpMatrix;
uniform mat4 mvMatrix;
uniform mat4 cameraMatrix;
uniform vec3 vHitPosition;

attribute vec4 vVertex;
attribute vec2 vTexCoord0;

varying vec2 vTex;

smooth out float fDistance;

void main( void )
{
	vTex = vTexCoord0;
	gl_Position = mvpMatrix * vVertex;
	
	vec4 vPosition4 = mvMatrix * vVertex;
	vec3 vPosition3 = vPosition4.xyz / vPosition4.w;
	
	vec4 vHitPos4 = cameraMatrix * vec4( vHitPosition, 1.0f );
	vec3 vHitPos3 = vHitPos4.xyz;
	
	fDistance = length( vHitPos3 - vPosition3 );
}