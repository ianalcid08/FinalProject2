/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : Push.h
Author : Evan Wright

Description : 

Created :  07/25/2013
Modified : 07/25/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "..\Header Files\stdafx.h"
#include "..\Header Files\Push.h"
#include "..\Header Files\CollisionLib.h"
//SoundManager includes....................................
#include "../../Includes/Sound/Source/Wwise_IDs.h"
#include "../../Includes/Sound/Source/AudioSystemWwise.h"
//.........................................................
#include "../Header Files/Renderer.h"
#include "../Header Files/AssetManager.h"

CPush::CPush( void )
{
	// Set type to push.
	this->SetType(ET_PUSH);

	// default orientation.
	GetRenderNode().SetUpVector( 0.0f, 1.0f, 0.0f );
	GetRenderNode().SetRightVector( 1.0f, 0.0f, 0.0f );
	GetRenderNode().SetForwardVector( 0.0f, 0.0f, 1.0f );

	Vec3f pos = { 0.0f, 0.0f, 0.0f };
	this->SetCollisionEntity(new Sphere(25.0f, pos));
}

CPush::~CPush( void )
{

}

void CPush::Initialize( CBaseEntity* _pusher )
{
	m_Pusher = _pusher;

	/*Vec3f sFwd;
	glsCopyVector3( sFwd, m_Pusher->GetRenderNode().m_vForward );
	glsScaleVector3( sFwd, 8.0f );
	glsAddVectors3(this->GetRenderNode().m_vPosition, m_Pusher->GetRenderNode().m_vPosition, sFwd);*/
	GetRenderNode().SetPosition( m_Pusher->GetRenderNode().GetPosition() );
	glsCopyVector3(*this->GetPosition(), this->GetRenderNode().GetPosition());

	m_DisplayTimer = 0.1f;
}

void CPush::InitShaders()
{
	//GetRenderNode().triangleBatch = &CAssetManager::GetInstance()->m_vTriangleBatches[PUSH_TB];
	GetRenderNode().SetTriangleBatch( nullptr );
	/*GetRenderNode().hShader = CAssetManager::GetInstance()->m_vShaderHandles[TEXTURE_ONLY];
	GetRenderNode().hUniformMVP = CAssetManager::GetInstance()->m_vUniformMVPHandles[TEXTURE_ONLY];
	GetRenderNode().hTextureUnit0 = CAssetManager::GetInstance()->m_vTextureUnit0Handles[TEXTURE_ONLY];
	GetRenderNode().hUniformColor = CAssetManager::GetInstance()->m_vUniformColorHandles[TEXTURE_ONLY];*/
}

// IEntity interface:
void CPush::Release( void )
{

}

bool CPush::Update( float _elapsedTime )
{
	m_DisplayTimer -= _elapsedTime;
	if( m_DisplayTimer < 0.0f )
	{
		this->SetIsActive(false);
		this->SetIsAlive(false);

	}
	else
	{
		/*Vec3f sFwd;
		glsCopyVector3( sFwd, m_Pusher->GetRenderNode().m_vForward );
		glsScaleVector3( sFwd, 8.0f );
		glsAddVectors3(this->GetRenderNode().m_vPosition, m_Pusher->GetRenderNode().m_vPosition, sFwd);*/
		GetRenderNode().SetPosition( m_Pusher->GetRenderNode().GetPosition() );
		glsCopyVector3(*this->GetPosition(), this->GetRenderNode().GetPosition());
	}

	glsCopyVector3(((Sphere*)this->GetCollisionEntity())->m_Center, this->GetRenderNode().GetPosition());
	return true;
}

void CPush::HandleReaction( void )
{

}
