// G-Buffer vertex Shader
#version 130

// Obviously, this has a long way to go.  However, for right now, we're just 
// test-rendering the diffuse component of everything.

uniform mat4 mMatrix;
uniform mat4 mvpMatrix;
uniform mat3 normalMatrix;
attribute vec4 vVertex;
attribute vec3 vNormal;
attribute vec2 vTexCoord0;
out vec2 vTex;
out vec3 vVaryingNormal;
out vec4 vVaryingPosition;

void main( void )
{
	vTex = vTexCoord0;
	//vVaryingNormal = normalMatrix * vNormal;
	vVaryingNormal = (mMatrix * vec4( vNormal, 0.0 )).xyz;
	vVaryingPosition = mMatrix * vec4(vVertex.xyz, 1.0);	// World Space Position
	gl_Position = mvpMatrix * vVertex;						// Projection Space Position
}