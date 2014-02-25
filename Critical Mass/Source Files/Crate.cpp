/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : Crate.cpp
Author : Evan Wright

Description : 

Created :  07/16/2013
Modified : 08/13/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "../Header Files/stdafx.h"
#include "../Header Files/Crate.h"
#include "../Header Files/CollisionLib.h"
#include "../Header Files/CollisionEntity.h"
#include "../Header Files/HealthPickup.h"
#include "../Header Files/Explosion.h"
#include "../Header Files/AssetManager.h"
#include "../Header Files/MessageSystem.h"
//SoundManager includes....................................
#include "../../Includes/Sound/Source/Wwise_IDs.h"
#include "../../Includes/Sound/Source/AudioSystemWwise.h"
//.........................................................

CCrate::CCrate( void )
{
	this->SetType(ET_CRATE);
	this->m_Health = MAX_HP_CRATE;
	this->m_deathTimer = 0.0f;

	// Collisions
	Vec3f pos = {0.0f,0.0f,0.0f};
	this->SetCollisionEntity(new Sphere(0.5f,pos));
	Sphere collisionSphere = *((Sphere*)this->GetCollisionEntity());

	// Default Orientation.
	GetRenderNode().SetUpVector( 0.0f, 1.0f, 0.0f );
	GetRenderNode().SetRightVector( 1.0f, 0.0f, 0.0f );
	GetRenderNode().SetForwardVector( 0.0f, 0.0f, 1.0f );

	glsCopyVector3(m_MoveUp,		this->GetRenderNode().GetUpVector());
	glsCopyVector3(m_MoveRight,		this->GetRenderNode().GetRightVector());
	glsCopyVector3(m_MoveForward,	this->GetRenderNode().GetForwardVector());

	Vec3f tempVel = {0.0f,0.0f,0.0f};
	this->SetVelocity(tempVel);
}
void CCrate::Init(float _rotX, float _rotY)
{
	//this->SetIsActive(true);
	//this->SetIsAlive(true);
	//this->m_Health = MAX_HP_CRATE;
	//this->m_deathTimer = 0.0f;
	Reset();

	this->SetIsActive(true);
	this->SetIsAlive(true);
	this->SetBeingPulled(false);
	m_PrevPulled = false;
	Vec3f tempVel = {0.0f,0.0f,0.0f};
	this->SetVelocity(tempVel);

	// Calculation for the rotation of the crate
	Vec3f fwd, up, right, final;
	Matrix3x3f rotMatrixY, rotMatrixX;

	glsRotationMatrix3x3( rotMatrixY, (float)glsDegToRad(_rotY), 0.0f, 1.0f, 0.0f );
	glsRotateVector(fwd, this->GetRenderNode().GetForwardVector(), rotMatrixY );
	glsNormalizeVector3(fwd);

	glsRotationMatrix3x3( rotMatrixX, (float)glsDegToRad(_rotX), fwd[0], fwd[1], fwd[2] );
	glsRotateVector(up, this->GetRenderNode().GetUpVector(), rotMatrixX );
	glsNormalizeVector3(up);

	glsCrossProduct3( right, fwd, up );
	glsNormalizeVector3(right);

	/*GetRenderNode().SetUpVector( up );
	GetRenderNode().SetRightVector( right );
	GetRenderNode().SetForwardVector( fwd );*/

	GetRenderNode().SetUpVector( 0.0f, 1.0f, 0.0f );
	GetRenderNode().SetRightVector( 1.0f, 0.0f, 0.0f );
	GetRenderNode().SetForwardVector( 0.0f, 0.0f, 1.0f );

	Vec3f _moon;
	glsLoadVector3( _moon, 0.0f, 75.0f, 0.0f );

	glsScaleVector3( up, 76 );
	glsAddVectors3( final, up, _moon );
	GetRenderNode().SetPosition( final );
	glsCopyVector3(*this->GetPosition(),final);

	Vec3f id;
	glsLoadVector3( id, 0.0f, 0.0f, 0.0f );

	// Reorient Ze Matrix
	Vec3f newRight, newUp, newForward;
	glsSubtractVectors3(newUp, _moon, this->GetRenderNode().GetPosition() );
	float length = glsGetVectorLength3( newUp );
	glsNormalizeVector3( newUp );
	GetRenderNode().SetUpVector( newUp );

	glsCrossProduct3( newForward, this->GetRenderNode().GetRightVector(), newUp );
	glsNormalizeVector3( newForward );
	GetRenderNode().SetForwardVector( newForward );

	glsCrossProduct3( newRight, newUp, newForward);
	glsNormalizeVector3( newRight );
	GetRenderNode().SetRightVector( newRight );

	Vec3f move_up;
	glsCopyVector3(move_up, this->GetRenderNode().GetUpVector());
	glsScaleVector3(move_up, 0.5f);
	Vec3f newPos;
	glsAddVectors3(newPos, this->GetRenderNode().GetPosition(), move_up);
	this->GetRenderNode().SetPosition(newPos);

	glsCopyVector3(m_startingPt,this->GetRenderNode().GetPosition());

	CMessageSystem::GetInstance()->SendMessageW(new CCreateBlueStrikeMessage(this->GetRenderNode().GetPosition()[0], this->GetRenderNode().GetPosition()[1], this->GetRenderNode().GetPosition()[2],true));
	glsCopyVector3(((Sphere*)(this->GetCollisionEntity()))->m_Center, final);	//HAX = no capsule to capsule collision
	
	Vec3f tempMoon = {0.0f,75.0f,0.0f};
	this->Adjust(tempMoon);
}
void CCrate::InitShaders()
{
	GetRenderNode().SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[CRATE_TB] );
	GetRenderNode().SetDiffuseTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_CRATE).m_DiffuseHandle );
	GetRenderNode().SetSpecularTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_CRATE).m_hTextureSpecular );
	GetRenderNode().SetEmissiveTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_CRATE).m_hTextureEmissive );
	GetRenderNode().SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[TEX_POINT_LIGHT] );
	GetRenderNode().SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[TEX_POINT_LIGHT] );
	GetRenderNode().SetMVUniformHandle( CAssetManager::GetInstance()->m_vUniformMVHandles[TEX_POINT_LIGHT] );
	GetRenderNode().SetNormalMatrixUniformHandle( CAssetManager::GetInstance()->m_vUniformNormalHandles[TEX_POINT_LIGHT] );
	GetRenderNode().SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[TEX_POINT_LIGHT] );
	GetRenderNode().SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[TEX_POINT_LIGHT] );
	GetRenderNode().SetTextureUnitHandle1( CAssetManager::GetInstance()->m_vTextureUnit1Handles[TEX_POINT_LIGHT] );
	GetRenderNode().SetTextureUnitHandle2( CAssetManager::GetInstance()->m_vTextureUnit2Handles[TEX_POINT_LIGHT] );
	GetRenderNode().SetLightPositionUniformHandle( CAssetManager::GetInstance()->m_vUniformLightPosHandles[0][TEX_POINT_LIGHT], 0 );
}
void CCrate::Reset()
{
	//this->SetIsActive(false);
	//this->SetIsAlive(false);
	this->m_deathTimer = 0.0f;
	this->m_Health = MAX_HP_CRATE;
	this->m_AmIBeingPulled = false;
	m_PrevPulled = false;
	m_startingPt[0] = 0.0f;
	m_startingPt[1] = 1.0f;
	m_startingPt[2] = 0.0f;
	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	Vec3f tempVel = {0.0f,0.0f,0.0f};
	this->SetVelocity(tempVel);

	this->SetType(ET_CRATE);

	// Default Orientation.
	GetRenderNode().SetUpVector( 0.0f, 1.0f, 0.0f );
	GetRenderNode().SetRightVector( 1.0f, 0.0f, 0.0f );
	GetRenderNode().SetForwardVector( 0.0f, 0.0f, 1.0f );

	glsCopyVector3(m_MoveUp,		this->GetRenderNode().GetUpVector());
	glsCopyVector3(m_MoveRight,		this->GetRenderNode().GetRightVector());
	glsCopyVector3(m_MoveForward,	this->GetRenderNode().GetForwardVector());
}
void CCrate::TakeDamage(int _damage)
{
	if(m_Health > 0)
	{
		m_Health -= _damage;

		if( m_Health <= 0)
		{
			//this->SetIsActive(false);
			//this->SetIsAlive(false);
			m_deathTimer = 0.5f;
//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_3D_SMASHCRATE);
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_CRATE_DESTROY);
					//AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::);


			// death particles!!
			CMessageSystem::GetInstance()->SendMessageW(new CCreateVFXMessage(EMITTER_EXPLOSION,this,0.0f,0.0f,0.0f));
		}
	}
}
// IEntity interface:
void CCrate::Release( void )
{
	
	CMovingEntity::Release();
}

bool CCrate::Update( float _elapsedTime )
{
	//CBaseEntity::Update(_elapsedTime);
	//CMovingEntity::Update(_elapsedTime);

	if(m_Health <= 0 && m_deathTimer >= 0.0f)
	{
		
		m_deathTimer -= _elapsedTime;
		Vec3f pos, move;
		glsCopyVector3(move,this->GetRenderNode().GetUpVector());
		glsScaleVector3(move,5.5f * _elapsedTime);
		glsAddVectors3(pos,GetRenderNode().GetPosition(),move);
		GetRenderNode().SetPosition(pos);
		
		if(m_deathTimer < 0.0f)
		{
			//CHealthPickup* pickup = nullptr;
			//m_OF->Create(ET_HEALTH_PICKUP_LARGE,(IEntity**)&pickup);
			//pickup->SetPosition(m_startingPt);
			//glsCopyVector3(	pickup->GetRenderNode().m_vPosition, m_startingPt );
			//CMessageSystem::GetInstance()->SendMessage(new CCreateHealthPickupMessage(m_startingPt[0],m_startingPt[1],m_startingPt[2]));
			CMessageSystem::GetInstance()->SendMessage(new CCreateHealthPickupMessage((*this->GetPosition())[0],(*this->GetPosition())[1],(*this->GetPosition())[2]));

			Reset();
			this->SetIsActive(false);
			this->SetIsAlive(false);
			//m_deathTimer = 0.0f;
			return false;
		}
		return true;
	}

	// if the crate is being pulled
	if(m_PrevPulled)
	{
		(*this->GetVelocity())[0] = this->GetRenderNode().GetForwardVector()[0] * SMALL_ENEMY_SPEED_BEING_PULLED;
		(*this->GetVelocity())[1] = this->GetRenderNode().GetForwardVector()[1] * SMALL_ENEMY_SPEED_BEING_PULLED;
		(*this->GetVelocity())[2] = this->GetRenderNode().GetForwardVector()[2] * SMALL_ENEMY_SPEED_BEING_PULLED;
		UpdateVelocity();
		
	}
	else
	{
		Vec3f tempVel = {0.0f,0.0f,0.0f};
		this->SetVelocity(tempVel);
	}
	CMovingEntity::Update(_elapsedTime);
	//if( m_PrevPulled != m_AmIBeingPulled && this->m_AmIBeingPulled == false )
	//{
	//	
	//	CMovingEntity::Update(_elapsedTime);
	//	/*m_PrevPulled = m_AmIBeingPulled;
	//	m_AmIBeingPulled = false;*/
	//}
	
	//UpdateVelocity();
	m_PrevPulled = m_AmIBeingPulled;
	m_AmIBeingPulled = false;
	
	//glsCopyVector3(*this->GetPosition(),this->GetRenderNode().GetPosition());
	this->GetRenderNode().SetPosition((*this->GetPosition())[0],(*this->GetPosition())[1],(*this->GetPosition())[2]);
	glsCopyVector3(((Sphere*)this->GetCollisionEntity())->m_Center, this->GetRenderNode().GetPosition());

	return true;
}

void CCrate::HandleReaction( void )
{
	std::vector<IEntity*> Col = this->GetCollisions();
	for(std::vector<IEntity*>::iterator iter = Col.begin(); iter != Col.end(); ++iter)
	{
		switch ((*iter)->GetType())
		{
		case ET_EXPLOSION:
			{
				m_deathTimer = 0.0f;
				TakeDamage(MAX_HP_CRATE);
			}
			break;
		case ET_BULLET_PLAYER:
			{
				TakeDamage(MAX_HP_CRATE);
			}
			break;
		case ET_LASER:
			{
				TakeDamage(MAX_HP_CRATE);
			}
			break;
		
		case ET_PUSH:
			{
				m_deathTimer = 0.0f;
				TakeDamage(MAX_HP_CRATE);
			}
			break;
		case ET_PULL:
			{
				this->SetBeingPulled(true);
			}
			break;
		case ET_PLAYER:
			{
				bool temp = this->GetBeingPulled();
				int x = 0;
				if(m_PrevPulled)
				{
					this->TakeDamage(MAX_HP_CRATE);
				}
			}
			break;
		};
	}

	Vec3f tempMoon = {0.0f,75.0f,0.0f};
	this->Adjust(tempMoon);
}
void CCrate::CrateBeams(Vec3f _pos)
{

}
void CCrate::UpdateVelocity( void )
{
	Vec3f totalUp, totalForward, totalRight, totalVel;
	glsCopyVector3( totalUp,		m_MoveUp );
	glsCopyVector3( totalRight,		m_MoveRight );
	glsCopyVector3( totalForward,	m_MoveForward );
	glsLoadVector3( totalVel, 0.0f, 0.0f, 0.0f );

	glsScaleVector3( totalUp,		1.0f );
	glsScaleVector3( totalRight,	*this->GetVelocity()[0] );
	glsScaleVector3( totalForward,	*this->GetVelocity()[2] );

	glsAddVectors3( totalVel, totalVel, totalUp );
	glsAddVectors3( totalVel, totalVel, totalRight );
	glsAddVectors3( totalVel, totalVel, totalForward );
}
