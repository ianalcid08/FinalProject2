#version 330


uniform mat4 mvpMatrix;
uniform mat4 mvMatrix;
uniform mat4 cameraMatrix;
uniform mat3 normalMatrix;
uniform vec3 vLightPosition;
uniform mat4 mSkin[30];

attribute vec4 vVertex;
attribute vec3 vNormal;
attribute vec2 vTexCoord0;
in uvec4 nJointIndex;
in vec4 fJointWeight;

out vec2 vTex;

smooth out vec3 vVaryingNormal;
smooth out vec3 vVaryingLightDir;
smooth out vec3 vVaryingToEye;
smooth out float fDistance;

void main( void )
{
	vec4 newPos;
	vec4 pos = vec4(vVertex.xyz, 1.0f);
	
	newPos = mSkin[nJointIndex[0]] * pos * fJointWeight.x;
	newPos += mSkin[nJointIndex[1]] * pos * fJointWeight.y;
	newPos += mSkin[nJointIndex[2]] * pos * fJointWeight.z;
	newPos += mSkin[nJointIndex[3]] * pos * fJointWeight.w;
	vVaryingNormal = normalMatrix * vNormal;
	
	vec4 vPosition4 = mvMatrix * newPos;
	vec3 vPosition3 = vPosition4.xyz / vPosition4.w;
	
	vec4 vLightPos4 = cameraMatrix * vec4( vLightPosition, 1.0f );
	vec3 vLightPos3 = vLightPos4.xyz;
	
	vVaryingLightDir = vPosition3 - vLightPos3;
	fDistance = length( vVaryingLightDir );
	vVaryingLightDir = normalize( vVaryingLightDir );
	
	vVaryingToEye = normalize( vec3( cameraMatrix[0][3], cameraMatrix[1][3], cameraMatrix[2][3] ) - vPosition3 );
	
	vTex = vTexCoord0;
	gl_Position = mvpMatrix * newPos;
}