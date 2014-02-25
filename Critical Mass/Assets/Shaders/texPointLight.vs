// Textured point light vertex shader
#version 130

uniform mat4 mvpMatrix;
uniform mat4 mvMatrix;
uniform mat4 cameraMatrix;
uniform mat3 normalMatrix;
uniform vec3 vLightPosition;

attribute vec4 vVertex;
attribute vec3 vNormal;
attribute vec2 vTexCoord0;

out vec2 vTex;

smooth out vec3 vVaryingNormal;
smooth out vec3 vVaryingLightDir;
smooth out vec3 vVaryingToEye;
smooth out float fDistance;

void main( void )
{
	vVaryingNormal = normalMatrix * vNormal;
	
	vec4 vPosition4 = mvMatrix * vVertex;
	vec3 vPosition3 = vPosition4.xyz / vPosition4.w;
	
	vec4 vLightPos4 = cameraMatrix * vec4( vLightPosition, 1.0f );
	vec3 vLightPos3 = vLightPos4.xyz;
	
	vVaryingLightDir = vPosition3 - vLightPos3;
	fDistance = length( vVaryingLightDir );
	vVaryingLightDir = normalize( vVaryingLightDir );
	
	vVaryingToEye = normalize( vec3( cameraMatrix[0][3], cameraMatrix[1][3], cameraMatrix[2][3] ) - vPosition3 );
	
	vTex = vTexCoord0;
	gl_Position = mvpMatrix * vVertex;
}
