/*
	File:		LightPole.cpp
	Author:		Bob Pasekoff
	Updated:	8/15/2013
*/

#include "../Header Files/stdafx.h"
#include "../Header Files/LightPole.h"
#include "../Header Files/CollisionEntity.h"
#include "../Header Files/CollisionLib.h"
#include "../Header Files/AssetManager.h"

CLightPole::CLightPole( void )
{
	CBaseEntity::SetType( ET_LIGHT_POLE );
}

void CLightPole::Initialize( double _X, double _Y)
{
	// Set up the shader
	GetRenderNode().SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[LIGHT_POLE_TB] );
	GetRenderNode().SetDiffuseTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_LIGHT_POLE).m_DiffuseHandle );
	//GetRenderNode().hTextureSpecular = CAssetManager::GetInstance()->GetObjData(ET_LIGHT_POLE).m_hTextureSpecular;
	GetRenderNode().SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[TEXTURE_ONLY] );
	GetRenderNode().SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[TEXTURE_ONLY] );
	//GetRenderNode().hUniformNormal = CAssetManager::GetInstance()->m_vUniformNormalHandles[TEXTURE_ONLY];
	GetRenderNode().SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[TEXTURE_ONLY] );
	GetRenderNode().SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[TEXTURE_ONLY] );
	//GetRenderNode().hTextureUnit1 = CAssetManager::GetInstance()->m_vTextureUnit1Handles[TEXTURE_ONLY];

	// Set an initial position and orientation
	Vec3f pos = { 0.0f, 75.0f, 0.0f },
		right = { 1.0f, 0.0f, 0.0f },
		up, u = { 0.0f, 1.0f, 0.0f },
		forward, f = { 0.0f, 0.0f, 1.0f };

	Matrix3x3f rotation;
	glsRotationMatrix3x3( rotation, (float)glsDegToRad( _Y ), 0.0f, 1.0f, 0.0f );
	glsRotateVector( forward, f, rotation );
	glsNormalizeVector3( forward );
	glsCrossProduct3( right, forward, u );
	glsNormalizeVector3( right );

	glsRotationMatrix3x3( rotation, (float)glsDegToRad( _X ), right[0], right[1], right[2] );
	glsCopyVector3( f, forward );
	glsRotateVector( forward, f, rotation );
	glsCrossProduct3( up, right, forward );

	glsNormalizeVector3( right );
	glsNormalizeVector3( up );
	glsNormalizeVector3( forward );

	pos[0] += up[0] * -75.0f;
	pos[1] += up[1] * -75.0f;
	pos[2] += up[2] * -75.0f;

	SetPosition( pos );
	GetRenderNode().SetPosition( pos );
	GetRenderNode().SetRightVector( right );
	GetRenderNode().SetUpVector( up );
	GetRenderNode().SetForwardVector( forward );

	// Set up the actual point light.
	Vec4f white = { 1.0f, 1.0f, 1.0f, 1.0f };
	glsCopyVector3( m_pointLight.m_Position, GetRenderNode().GetPosition() );
	glsScaleVector3( up, 12.0f );  // This is how high off the ground the light actually is.
	glsScaleVector3( m_pointLight.m_Position, -1.0f );
	glsAddVectors3( m_pointLight.m_Position, m_pointLight.m_Position, up );
	m_pointLight.Init( m_pointLight.m_Position, white );
}

// IEntity interface:
void CLightPole::Release( void )
{
	CBaseEntity::Release();
}

bool CLightPole::Update( float _elapsedTime )
{
	CBaseEntity::Update(_elapsedTime);

	return true;
}

void CLightPole::HandleReaction( void )
{

}
