#version 330

uniform mat4 mvpMatrix;
uniform mat3 normalMatrix;
uniform mat4 mSkin[30];

in vec4 vVertex;
in vec2 vTexCoord0;
in vec3 vNormal;
in uvec4 nJointIndex;
in vec4 fJointWeight;

out vec2 vTex;
smooth out vec3 vVaryingNormal;

void main( void )
{
	vVaryingNormal = normalMatrix * vNormal;
	vTex = vTexCoord0;
	
	vec4 newPos;
	vec4 pos = vec4(vVertex.xyz, 1.0f);
	
	newPos = mSkin[nJointIndex[0]] * pos * fJointWeight.x;
	newPos += mSkin[nJointIndex[1]] * pos * fJointWeight.y;
	newPos += mSkin[nJointIndex[2]] * pos * fJointWeight.z;
	newPos += mSkin[nJointIndex[3]] * pos * fJointWeight.w;
	
	gl_Position = mvpMatrix * newPos;
}