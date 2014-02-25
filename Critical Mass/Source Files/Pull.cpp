/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : Pull.h
Author : Evan Wright

Description : 

Created :  07/25/2013
Modified : 07/25/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "..\Header Files\stdafx.h"
#include "..\Header Files\Pull.h"
#include "..\header files\SmallEnemy.h"
#include "..\Header Files\Capsule.h"
#include "..\Header Files\GLSetup.h"
#include "..\Header Files\CollisionLib.h"
#include "..\Header Files\Player.h"
#include "..\Header Files\AIManager.h"
//SoundManager includes....................................
#include "../../Includes/Sound/Source/Wwise_IDs.h"
#include "../../Includes/Sound/Source/AudioSystemWwise.h"
//.........................................................
#include "../Header Files/AssetManager.h"

CPull::CPull( void )
{
	this->SetType(ET_PULL);
	// default orientation.
	GetRenderNode().SetUpVector( 0.0f, 1.0f, 0.0f );
	GetRenderNode().SetRightVector( 1.0f, 0.0f, 0.0f );
	GetRenderNode().SetForwardVector( 0.0f, 0.0f, -1.0f );
}
void CPull::Initialize( CBaseEntity* _destination, CBaseEntity* _target )
{
	m_Target = _target;
	m_Destination = _destination;
	//CSoundManager::GetInstance()->PlaySound(AK::EVENTS::PLAY_DX_2D_UNITCOMMANDS);

	// Initialize the collision data based on the 
	// radius, start, end.
	Vec3f start, endOffset, end;
	glsCopyVector3(start, m_Destination->GetRenderNode().GetPosition());

	// Get the end position by scaling the input's forward and adding it to start.
	glsCopyVector3(endOffset, m_Destination->GetRenderNode().GetForwardVector());
	glsScaleVector3(endOffset, 10);
	glsAddVectors3(end,endOffset,start);
	
	CCollisionEntity* entity = GetCollisionEntity();
	if( entity )
	{
		delete entity;
	}

	this->SetCollisionEntity(new Capsule(Segment(start, end), 4.0f));

	GetRenderNode().SetPosition( start );
	GetRenderNode().SetForwardVector( _destination->GetRenderNode().GetForwardVector() );
	GetRenderNode().SetUpVector( _destination->GetRenderNode().GetUpVector() );
	GetRenderNode().SetRightVector( _destination->GetRenderNode().GetRightVector() );
	
	m_HasPulled = false;
}
void CPull::InitShaders()
{
	GetRenderNode().SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[PULL_CONE_TB] );
	GetRenderNode().SetDiffuseTextureHandle(  CAssetManager::GetInstance()->GetObjData(ET_PULL).m_DiffuseHandle );
	GetRenderNode().SetColor(0.0f,0.0f,1.0f,0.8f);

	GetRenderNode().SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[SCROLLING_UV] );
	GetRenderNode().SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[SCROLLING_UV] );
	GetRenderNode().SetTimeUniformHandle( CAssetManager::GetInstance()->m_vUniformTimeHandles[SCROLLING_UV] );
	GetRenderNode().SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[SCROLLING_UV] );
	GetRenderNode().SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[SCROLLING_UV] );
	GetRenderNode().SetEnableBlending( true );
}
// IEntity interface:
void CPull::Release( void )
{
}

bool CPull::Update( float _elapsedTime )
{
	if( m_Target != nullptr && m_Destination != nullptr )
	{
		// Move the target to the destination
		((CSmallEnemy*)m_Target)->TurnTo(*m_Destination->GetPosition());
	}
	
	if( m_Destination != nullptr )
	{
		// Update the pulls area based on the destinations forward
		Vec3f start, endOffset, end;
		glsCopyVector3(start, m_Destination->GetRenderNode().GetPosition());

		glsCopyVector3(endOffset, m_Destination->GetRenderNode().GetForwardVector());
		glsScaleVector3(endOffset, 10);
		glsAddVectors3(end,endOffset,start);

		// Update the collision entity
		glsCopyVector3(((Capsule*)(this->GetCollisionEntity()))->m_Segment.m_Start, start);
		glsCopyVector3(((Capsule*)(this->GetCollisionEntity()))->m_Segment.m_End, end);

		Vec3f up, final;
		glsCopyVector3(up, m_Destination->GetRenderNode().GetUpVector());
		glsScaleVector3(up, -2.0f);
		glsAddVectors3(final, up, start);

		GetRenderNode().SetPosition( final );
		GetRenderNode().SetForwardVector( m_Destination->GetRenderNode().GetForwardVector() );
		GetRenderNode().SetUpVector( m_Destination->GetRenderNode().GetUpVector() );
		GetRenderNode().SetRightVector( m_Destination->GetRenderNode().GetRightVector() );
	}

	return true;
}

void CPull::HandleReaction( void )
{
	std::vector<IEntity*> Col = this->GetCollisions();
	for(std::vector<IEntity*>::iterator iter = Col.begin(); iter != Col.end(); ++iter)
	{
		switch( (*iter)->GetType() )
		{
		case ET_SMALLENEMY:
			{
				// Set small enemy to state: "Being pulled"
				if( m_Destination != nullptr )
				{
					m_Target = ((CBaseEntity*)(*iter));
					//((CSmallEnemy*)m_Target)->SetState(ST_VULNERABLE); 
					//int type = ((CSmallEnemy*)_other)->GetPowerupType();
					//((CPlayer*)m_Destination)->GetGun().Store( type );
					//((CPlayer*)m_Destination)->GetGun().Process();
					//((CPlayer*)m_Destination)->HasPulled();
				}
			}
			break;
		case ET_CRATE:
			{
				if(m_Destination != nullptr)
				{
					m_Target = ((CBaseEntity*)(*iter));
				}
			}
			break;
		case ET_PLAYER:
			{
				if( m_Destination != nullptr && m_Destination->GetType() == ET_PLAYER )
				{
					// Pull was successfull 

					// DESTROY THE TARGET!
					if( m_Target != nullptr )
					{
						// Get Enemy powerup type and set player's powerup to that!
						//int type = ((CSmallEnemy*)_other)->GetPowerupType();
						//((CPlayer*)m_Destination)->GetGun().Store( type );
						//((CPlayer*)m_Destination)->GetGun().Process();
						//((CPlayer*)m_Destination)->HasPulled();
					}
				}
			}
		}
	}
}
