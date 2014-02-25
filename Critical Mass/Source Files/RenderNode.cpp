/*
	File:		RenderNode.cpp
	Author:		Bob Pasekoff
	Updated:	7/17/2013

*/
#include "../Header Files/stdafx.h"
#include "../Header Files/RenderNode.h"
#include "../Header Files/AssetManager.h"


CRenderNode::CRenderNode( void )
{
	// World position, rotation, and scale
	glsLoadVector3( m_vPosition, 0.0f, 0.0f, 0.0f );
	glsLoadVector3( m_vUp, 0.0f, 1.0f, 0.0f );
	glsLoadVector3( m_vRight, 1.0f, 0.0f, 0.0f );
	glsLoadVector3( m_vForward, 0.0f, 0.0f, -1.0f );
	glsLoadVector4( m_color, 1.0f, 1.0f, 1.0f, 1.0f );
	m_fScale = 1.0f;

	// Texture and shader handles
	hShader = 0;
	hTextureDiffuse = 0;
	hTextureNormal = 0;
	hTextureSpecular = 0;
	hTextureEmissive = 0;
	hTextureUnit0 = 0;
	hTextureUnit1 = 0;
	hTextureUnit2 = 0;
	hTextureUnit3 = 0;

	// Uniform handles - These will likely change
	m_numLights	= 0;
	hUniformLightPos[0] = 0;
	hUniformLightPos[1] = 0;
	hUniformLightPos[2] = 0;
	hUniformLightPos[3] = 0;
	hUniformLightPos[4] = 0;
	hUniformLightPos[5] = 0;
	hUniformLightPos[6] = 0;
	hUniformLightPos[7] = 0;
	hUniformMVP = 0;			
	hUniformMV = 0;	
	hUniformProj = 0;
	hUniformColor = 0;			
	hUniformNormal = 0;			
	hUniformInverseCamera = 0;
	hUniformSkinningMatrix = 0;
	hUniformScaleMatrix = 0;
	m_dissolveFactor = 1.0f;
	m_SkinningMatrix = nullptr;

	// Special considerations
	renderBackFacing = false;
	enableBlending = false;
	renderWireFrame = false;
}

