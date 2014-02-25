/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : Depot.cpp
Author : Corey Morehead

Description : This Class will have the set up for the depot class.

Created :  07/23/2013
Modified : 07/23/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "../Header Files/stdafx.h"
#include "../Header Files/Depot.h"
#include "../Header Files/CollisionEntity.h"
#include "../Header Files/Definitions.h"
#include "../Header Files/GLSetup.h"
#include "../Header Files/CollisionLib.h"
#include "../Header Files/Shield.h"
#include "../Header Files/ObjectFactory.h"
//SoundManager includes....................................
#include "../../Includes/Sound/Source/Wwise_IDs.h"
#include "../../Includes/Sound/Source/AudioSystemWwise.h"
//.........................................................
#include "../Header Files/AssetManager.h"
#include "../Header Files/CollisionLib.h"
#include "../Header Files/MessageSystem.h"

CDepot::CDepot( void )
{
	CBaseEntity::SetType(ET_DEPOT);

	// COLLISION 
	Vec3f pos = { 0.0f, 0.0f, 0.0f };
	this->SetCollisionEntity(new Sphere(1.0f, pos));
	this->SetDepotExp(EXP_DEPOT);
}

void CDepot::Initialize( double _X, double _Y )
{
	DestroyShield();

	// default orientation.
	GetRenderNode().SetUpVector( 0.0f, 1.0f, 0.0f );
	GetRenderNode().SetRightVector( 1.0f, 0.0f, 0.0f );
	GetRenderNode().SetForwardVector( 0.0f, 0.0f, 1.0f );

	this->SetIsActive(true);
	this->SetIsAlive(true);

	this->m_health = MAX_HP_DEPOT;
	this->m_State = ACTIVATED;

	m_rotX = (float)_X;
	m_rotY = (float)_Y;

	//Calculation for the rotation of the Depot
	Vec3f fwd, up, right, final;
	//m_Fwd, m_Up, m_Right, m_Final
	Matrix3x3f rotMatrixY, rotMatrixX;

	glsRotationMatrix3x3( rotMatrixY, (float)glsDegToRad(_Y), 0.0f, 1.0f, 0.0f );
	glsRotateVector(fwd, this->GetRenderNode().GetForwardVector(), rotMatrixY );
	glsNormalizeVector3(fwd);

	glsRotationMatrix3x3( rotMatrixX, (float)glsDegToRad(_X), fwd[0], fwd[1], fwd[2] );
	glsRotateVector(up, this->GetRenderNode().GetUpVector(), rotMatrixX );
	glsNormalizeVector3(up);

	glsCrossProduct3( right, fwd, up );
	glsNormalizeVector3(right);

	GetRenderNode().SetUpVector( up );
	GetRenderNode().SetRightVector( right );
	GetRenderNode().SetForwardVector( fwd );

	Vec3f _moon;
	glsLoadVector3( _moon, 0.0f, 75.0f, 0.0f );

	glsScaleVector3( up, -75 );
	glsAddVectors3( final, up, _moon );
	GetRenderNode().SetPosition( final );
	
	glsCopyVector3(((Sphere*)(this->GetCollisionEntity()))->m_Center, final);
	
	Vec3f id;
	glsLoadVector3( id, 0.0f, 0.0f, 0.0f );

	// Reorient ze matrix
	Vec3f newRight, newUp, newForward;
	glsSubtractVectors3( newUp, _moon, this->GetRenderNode().GetPosition() );
	float length = glsGetVectorLength3( newUp );
	glsNormalizeVector3( newUp );
	GetRenderNode().SetUpVector( newUp );

	glsCrossProduct3( newForward, this->GetRenderNode().GetRightVector(), newUp );
	glsNormalizeVector3( newForward );
	GetRenderNode().SetForwardVector( newForward );

	glsCrossProduct3( newRight, newUp, newForward );
	glsNormalizeVector3( newRight );
	GetRenderNode().SetRightVector( newRight );

	glsCopyVector3(*this->GetPosition(), GetRenderNode().GetPosition());

	((Sphere*)this->GetCollisionEntity())->m_Radius = 0.0f;

	m_Deactivated_Sound = false;
	m_Activated_Sound = false;
	m_State = ACTIVATED;
	m_DeactivateTimer	= 0.0f;
	m_ActivateTimer		= 0.0f;
} 
void CDepot::InitShaders( void )
{
	GetRenderNode().SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[MINING_STATION_TB] );
	GetRenderNode().SetDiffuseTextureHandle(  CAssetManager::GetInstance()->GetObjData(ET_DEPOT).m_DiffuseHandle );
	GetRenderNode().SetSpecularTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_DEPOT).m_hTextureSpecular );
	GetRenderNode().SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[ANIMATED_ONLY] );
	GetRenderNode().SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[ANIMATED_ONLY] );
	GetRenderNode().SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[ANIMATED_ONLY] );
	GetRenderNode().SetSkinningMatrixUniformHandle( CAssetManager::GetInstance()->m_vUniformJointHandles[ANIMATED_ONLY] );
	GetRenderNode().SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[ANIMATED_ONLY] );
	GetRenderNode().SetColor(1.0f, 1.0f, 1.0f, 1.0f);

	this->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_DEPOT, DepotAnim_Deactivated_Idle), 0.1f);
}

void CDepot::Reset( void )
{
	m_State = DEACTIVATED;
	if(m_Deactivated_Sound)
		AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_SFX_LOOP_DEPOT_RETREAT);
	if(m_Activated_Sound)
		AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_SFX_LOOP_DEPOT_RISING);
	m_Deactivated_Sound = false;
	m_Activated_Sound = false;
	this->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_DEPOT, DepotAnim_Deactivate), 0.1f);
}

void CDepot::Activate( bool _invulnerable )
{
	if (m_State == DEACTIVATED)
	{
		m_State = ACTIVATED;
		AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_SFX_LOOP_DEPOT_RETREAT);
		AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_LOOP_DEPOT_RISING);
		m_Activated_Sound = true;
		m_Deactivated_Sound = false;
		this->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_DEPOT, DepotAnim_Activate), 0.1f);

		// Create a shield
		CMessageSystem::GetInstance()->SendMessage(new CCreateShieldMessage(m_rotX,m_rotY,this,ET_DEPOT,_invulnerable));
	}
}

void CDepot::Deactivate( void )
{
	if(m_State == ACTIVATED)
	{
		m_State = DEACTIVATED;
		AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_LOOP_DEPOT_RETREAT);
		AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_SFX_LOOP_DEPOT_RISING);
		m_Deactivated_Sound = true;
		m_Activated_Sound = false;
		this->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_DEPOT, DepotAnim_Deactivate), 0.1f);

		m_DeactivateTimer = 5.0f;
	}

}

// IEntity interface:
void CDepot::Release( void )
{
	CBaseEntity::Release();
}

bool CDepot::Update( float _elapsedTime )
{
	CLivingEntity::Update(_elapsedTime);

	if(m_Shield != nullptr && ((CShield*)m_Shield)->GetHealth() <= 0)
		m_health = 0;
	else if(m_Shield == nullptr)
		Deactivate();
	
	if( m_State == DEACTIVATED )
	{
		if( m_Shield && ((CShield*)m_Shield)->GetRenderNode().GetDissolveFactor() <= 0.0f )
		{
			DestroyShield();
			
			if(m_Deactivated_Sound)
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_SFX_LOOP_DEPOT_RETREAT);
			m_Deactivated_Sound = false;

			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_EXPLOSION_INTENSITY3);
			CMessageSystem::GetInstance()->SendMessage(new CCreateVFXMessage(GEO_EMITTER_ENERGY_RING,this,0.0f,0.0f,0.0f));
		}
	}

	if( m_State == ACTIVATED )
	{
		if( m_Deactivated_Sound )
		{
			m_Deactivated_Sound = false;
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_SFX_LOOP_DEPOT_RETREAT);
		}
	}
	else if( m_State == DEACTIVATED )
	{
		if( m_Activated_Sound )
		{
			m_Activated_Sound = false;
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_SFX_LOOP_DEPOT_RISING);
		}
	}

	if( m_State == ACTIVATED )
	{		
		if( m_Shield != nullptr )
		{
			if( ((CShield*)m_Shield)->GetCollisionSize() < 25.0f  )
				((CShield*)m_Shield)->ResizeCollisionEntity(((CShield*)m_Shield)->GetCollisionSize() + 35.0f * _elapsedTime);
		}

		if(m_Activated_Sound)
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_SFX_LOOP_DEPOT_RISING);
		m_Activated_Sound = false;
		
		if( m_Shield != nullptr )
			((CShield*)m_Shield)->ResizeCollisionEntity(25.0f);

		if( m_Shield != nullptr )
		{
			SetShieldPos(GetRenderNode().GetPosition());	
			((CShield*)m_Shield)->GetRenderNode().SetDissolveFactor( 1.0f );
		}
	}

	glsCopyVector3(((Sphere*)this->GetCollisionEntity())->m_Center, this->GetRenderNode().GetPosition());
	return true;
}

void CDepot::SetShieldPos( const Vec3f _pos )
{ 
	((CShield*)m_Shield)->GetRenderNode().SetPosition(_pos);
	glsCopyVector3(((Sphere*)((CShield*)m_Shield)->GetCollisionEntity())->m_Center, _pos);
}

void CDepot::DestroyShield( void ) 
{ 
	if(m_Shield != nullptr)
	{
		m_Shield->SetIsActive(false);
		m_Shield->SetIsAlive(false);
		m_Shield = nullptr;
	}
}

void CDepot::HandleReaction( void )
{
	
}


void CDepot::SwitchAnimation(void)
{
	//CAnimationManager* pAM = this->GetAnimationManager();
	//this->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_DEPOT, DepotAnim_Activate), 0.4f);
}