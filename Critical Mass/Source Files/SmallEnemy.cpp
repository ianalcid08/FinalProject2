/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : SmallEnemy.cpp
Author : Evan Wright

Description : The small enemy that will be the main way to gain powerups in our game.
			  He will be a minor challange to the player and is the only enemy that can
			  be pulled into the gun.

Created :  07/16/2013
Modified : 08/15/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "../Header Files/stdafx.h"
#include "../Header Files/SmallEnemy.h"
#include "../Header Files/CollisionLib.h"
#include "../Header Files/AssetManager.h"
#include "../Header Files/MessageSystem.h"
//SoundManager includes....................................
#include "../../Includes/Sound/Source/Wwise_IDs.h"
#include "../../Includes/Sound/Source/AudioSystemWwise.h"
//.........................................................

// HAX: Temp link to objectfactory. 
// Desc: Will remove when we can make messages to create objects.
#include "ObjectFactory.h"

#include "GamePlayState.h"
#include <iostream>
using std::cout;
using std::endl;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Default constructor
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
CSmallEnemy::CSmallEnemy(void)
{
	this->SetType(ET_SMALLENEMY);

	GetRenderNode().SetUpVector( 0.0f, 1.0f, 0.0f );
	GetRenderNode().SetRightVector( 1.0f, 0.0f, 0.0f );
	GetRenderNode().SetForwardVector( 0.0f, 0.0f, 1.0f );

	// For movement...
	glsCopyVector3(m_MoveUp,		this->GetRenderNode().GetUpVector());
	glsCopyVector3(m_MoveRight,		this->GetRenderNode().GetRightVector());
	glsCopyVector3(m_MoveForward,	this->GetRenderNode().GetForwardVector());

	// COLLISION DATA
	Vec3f pos = { 0.0f, 0.0f, 0.0f };
	this->SetCollisionEntity(new Sphere(2.0f, pos));
	this->SetHealth(MAX_HP_SMALL_ENEMY);

	// For initial spawning
	switch(rand()%2)
	{
	case 0:
		m_RightDirection = 1.0f;
		break;
	case 1:
		m_RightDirection = -1.0f;
		break;
	}
	
	this->SetExperienceValue(EXP_SMALL_ENEMY);
	m_YellowTimer = 0.0f;
	Reset();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Destructor
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CSmallEnemy::Release()
{
	CLivingEntity::Release();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Initializes all data for render node.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CSmallEnemy::Init()
{
	SetState(0);
	GetRenderNode().SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[SMALL_ENEMY_SHAPE_TB] );

	GetRenderNode().SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[TEX_POINT_LIGHT] );
	GetRenderNode().SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[TEX_POINT_LIGHT] );
	GetRenderNode().SetMVUniformHandle( CAssetManager::GetInstance()->m_vUniformMVHandles[TEX_POINT_LIGHT] );
	GetRenderNode().SetNormalMatrixUniformHandle( CAssetManager::GetInstance()->m_vUniformNormalHandles[TEX_POINT_LIGHT] );
	GetRenderNode().SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[TEX_POINT_LIGHT] );
	GetRenderNode().SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[TEX_POINT_LIGHT] );
	GetRenderNode().SetTextureUnitHandle1( CAssetManager::GetInstance()->m_vTextureUnit1Handles[TEX_POINT_LIGHT] );
	GetRenderNode().SetTextureUnitHandle2( CAssetManager::GetInstance()->m_vTextureUnit2Handles[TEX_POINT_LIGHT] );
	GetRenderNode().SetLightPositionUniformHandle( CAssetManager::GetInstance()->m_vUniformLightPosHandles[0][TEX_POINT_LIGHT], 0 );

	GetRenderNode().SetDiffuseTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_SMALLENEMY).m_DiffuseHandle );
	GetRenderNode().SetSpecularTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_SMALLENEMY).m_hTextureSpecular );
	GetRenderNode().SetEmissiveTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_SMALLENEMY).m_hTextureEmissive );

	GetRenderNode().SetDissolveFactor( 1.0f );
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Updates the small enemy!
Parameters:
[in] _elapsedTime - the time passed since the last frame.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool CSmallEnemy::Update( float _elapsedTime )
{

	
#if _DEBUG
	if(this->GetIsPaused())
		return true;
#endif

	m_ElapsedTime = _elapsedTime;

	if(m_SpawnTimer > 0.0f)
	{
		m_SpawnTimer -= _elapsedTime;
	}

	if( this->GetState() == Enemy_Dying )
	{
		// If the enemy is black do not sink into the planetoid.
		//	rather: make him blink then EXPLODE!
		if(GetLaserDeath() && GetPowerupType() != BLACK)
		{
			GetRenderNode().SetDissolveFactor( GetRenderNode().GetDissolveFactor() - _elapsedTime );
			if(GetRenderNode().GetDissolveFactor() <= 0.0f)
			{
				this->SetIsActive(false);
				this->SetIsAlive(false);			
				SetLaserDeath(false);
			}
		}
		else
		{
			if( this->GetPowerupType() == BLACK )
			{
				if( m_ExploderBlinkTimer > 0.0f )
				{
					static bool flip = false;
					if(m_ExploderBlinkTimer >= m_DeathTimer )
					{
						if(flip)
							this->GetRenderNode().SetColor(0.8f,0.0f,0.0f,0.1f);
						else
							this->GetRenderNode().SetColor(0.0f,0.0f,0.0f,0.1f);

						m_ExploderBlinkTimer -= SMALL_ENEMY_BLACK_BLINK_RATE;
						flip = !flip;
					}
				}
			}
			// Move the small enemy into the planet, so it doesn't
			// pop out of the game.
			else
			{
				Vec3f nPos, move;
				glsCopyVector3(move,GetRenderNode().GetUpVector());
				glsScaleVector3(move, 5.5f * _elapsedTime);
				glsAddVectors3(nPos,GetRenderNode().GetPosition(),move);
				GetRenderNode().SetPosition( nPos );
			}

			if( m_DeathTimer == SMALL_ENEMY_TIMER_DEATH && this->GetPowerupType() != BLACK )
			{
				CMessageSystem::GetInstance()->SendMessageW(new CCreateVFXMessage(EMITTER_EXPLOSION,this,0.0f,0.0f,0.0f));
			}
		
			// Decrement the death timer.
			m_DeathTimer -= _elapsedTime;
			m_YellowTimer = m_DeathTimer;

			if( glsGetVectorLength3( *this->GetVelocity() ) > 0.0f )
			{
				UpdateVelocity();
				// This is for knockback...
				Vec3f pos;
				glsCopyVector3( pos, *GetPosition() );
				pos[0] += (*GetVelocity())[0] * _elapsedTime;
				pos[1] += (*GetVelocity())[1] * _elapsedTime;
				pos[2] += (*GetVelocity())[2] * _elapsedTime;
				SetPosition(pos);
				GetRenderNode().SetPosition( pos );
				Vec3f zero = { 0.0f, 0.0f, 0.0f };
				SetVelocity( zero );
			}

			// Check if the timer is finished.
			// If so... KILL ZE ENEMY!
			if(m_DeathTimer <= 0.0f)
			{
				Kill();
			}
		}

		return true;
	}

	// Update laser invulnerablity timer
	if( m_LaserTimer > 0.0f )
	{
		m_LaserTimer -= _elapsedTime;
		if( m_LaserTimer < 0.0f )
			m_LaserTimer = 0.0f;
	}

	// Orient the movement vector
	glsCopyVector3(m_MoveUp, this->GetRenderNode().GetUpVector());
	glsCrossProduct3( m_MoveForward, m_MoveRight, m_MoveUp );
	glsNormalizeVector3( m_MoveForward );
	glsCrossProduct3( m_MoveRight, m_MoveUp, m_MoveForward );
	glsNormalizeVector3( m_MoveRight );

	// If you're being pushed, MOVE FAST!
	if( this->GetState() == Enemy_Pushed && m_IsPushed )
	{
		Vec3f zero = { 0.0f, 0.0f, 0.0f };
		SetVelocity( zero );
		(*this->GetVelocity())[0] = this->GetRenderNode().GetForwardVector()[0] * -SMALL_ENEMY_SPEED_BEING_PUSHED;
		(*this->GetVelocity())[1] = this->GetRenderNode().GetForwardVector()[1] * -SMALL_ENEMY_SPEED_BEING_PUSHED;
		(*this->GetVelocity())[2] = this->GetRenderNode().GetForwardVector()[2] * -SMALL_ENEMY_SPEED_BEING_PUSHED;

		if(m_PushTimer > 0.0f)
			m_PushTimer -= _elapsedTime;
		else
		{
			m_IsPushed = false;
			if( this->GetPowerupType() != BLACK )
			{
				this->SetState( Enemy_Stunned );
				m_StunTimer = SMALL_ENEMY_TIMER_STUN;
			}
			else
			{
				this->SetState( Enemy_Dying );
				m_DeathTimer = SMALL_ENEMY_TIMER_DEATH;
				m_ExploderBlinkTimer = SMALL_ENEMY_TIMER_DEATH;
			}
		}
	}


	if(m_SpawnTimer > 0.0f)
	{
		(*this->GetVelocity())[0] = this->GetRenderNode().GetRightVector()[0] * m_RightDirection * -SMALL_ENEMY_SPEED_MOVE;
		(*this->GetVelocity())[1] = this->GetRenderNode().GetRightVector()[1] * m_RightDirection * -SMALL_ENEMY_SPEED_MOVE;
		(*this->GetVelocity())[2] = this->GetRenderNode().GetRightVector()[2] * m_RightDirection * -SMALL_ENEMY_SPEED_MOVE;
	}

	
	// If the enemy is vulnerable: move slowly. *basically for pull.
	if( this->GetState() == Enemy_Vulnerable )
	{
		(*this->GetVelocity())[0] = this->GetRenderNode().GetForwardVector()[0] * SMALL_ENEMY_SPEED_BEING_PULLED;
		(*this->GetVelocity())[1] = this->GetRenderNode().GetForwardVector()[1] * SMALL_ENEMY_SPEED_BEING_PULLED;
		(*this->GetVelocity())[2] = this->GetRenderNode().GetForwardVector()[2] * SMALL_ENEMY_SPEED_BEING_PULLED;
		UpdateVelocity();
	}

	// Check if the enemy can move.
	if( this->GetState() != Enemy_Stunned && this->GetState() != Enemy_Idle )
	{
		UpdateVelocity();
	}

	// If the enemy isn't stunned update it!
	if( this->GetState() != Enemy_Stunned )
		CEnemy::Update(_elapsedTime);

	// If the enemy was being pulled and now isn't...
	// SET TO STUNNED! MWAHAHAHAAA!
	if( m_prevIsPulled != m_IsPulled && m_IsPulled == false )
	{
		this->SetState( Enemy_Stunned );
		m_StunTimer = SMALL_ENEMY_TIMER_STUN;
	}
	m_prevIsPulled = m_IsPulled;
	m_IsPulled = false;

	// If the enemy is stunned...
	if( this->GetState() == Enemy_Stunned )
	{
		m_StunTimer -= _elapsedTime;
		if( m_StunTimer <= 0.0f )
		{
			m_StunTimer = 0.0f;
			// If the timer is up and the enemy isn't black turn to idle.
			if( this->GetPowerupType() != BLACK )
				this->SetState(Enemy_Idle);
			// ELSE KILL IT!
			else
				Kill();
		}
	}

	// Death stuff:
	if(this->GetHealth() <= 0 && this->GetState() != Enemy_Dying)
	{
		if( this->GetPowerupType() != BLACK )
		{
			this->SetState(Enemy_Dying);
			m_DeathTimer = SMALL_ENEMY_TIMER_DEATH;
			m_ExploderBlinkTimer = SMALL_ENEMY_TIMER_DEATH;
//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_3D_DEATH);
		}
		else
		{
			this->SetState(Enemy_Dying);
			Kill();
		}
	}

	return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Kills the enemy... Sets all the necessary variables and creates an explosion
		 if the enemy is black.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CSmallEnemy::Kill(void)
{
	this->SetIsActive(false);
	this->SetIsAlive(false);
	this->SetIsDying(true);
	
	Reset();

	// Make death effect
	Vec3f pos;
	glsCopyVector3(pos, this->GetRenderNode().GetPosition());
	CMessageSystem::GetInstance()->SendMessageW(new CCreateVFXMessage(EMITTER_EXPLOSION,nullptr,pos[0],pos[1],pos[2]));

	// If the enemy is black then create an explosion!
	if(this->GetPowerupType() == BLACK)
	{
		CMessageSystem::GetInstance()->SendMessage(new CCreateExplosionMessage(this,true));
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Resets all the data for the small enemy, minus the orientation/position/rendernode.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CSmallEnemy::Reset(void)
{
	m_prevIsPulled	= false;
	m_IsPulled		= false;
	m_IsPushed		= false;
	m_StunTimer		= 0.0f;
	m_PushTimer		= 0.0f;
	m_ExploderBlinkTimer = 0.0f;
	m_ElapsedTime	= 0.0f;
	m_LaserTimer	= 0.0f;
	m_DeathTimer	= 5;
	m_SpawnTimer	= 2.5f;

	this->SetMoveSpeed(SMALL_ENEMY_SPEED_MOVE);
	this->SetState(Enemy_Charging);
	this->SetHealth(MAX_HP_SMALL_ENEMY);

	GetRenderNode().SetEnableBlending( false );
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Handles the collision reaction.
Parameters:
[in] _other - the entity to react against.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CSmallEnemy::HandleReaction( void )
{
	bool SmallCol = false;
	Vec3f sumMove = { 0.0f, 0.0f, 0.0f };
	std::vector<IEntity*> Col = this->GetCollisions();
	for(std::vector<IEntity*>::iterator iter = Col.begin(); iter != Col.end(); ++iter)
	{
		switch((*iter)->GetType())
		{
		case ET_SMALLENEMY:

			if(this->GetState() == Enemy_Attacking || ((CSmallEnemy*)(*iter))->GetState() == Enemy_Attacking)
				continue;

		case ET_CRATE:
		case ET_REDBARREL:
		case ET_MEDIUMENEMY:
		case ET_SHIELD:
			{
				// Don't react if we just spawned, unless we're colliding with another
				// small enemy.
				if(m_SpawnTimer > 0.0f && (*iter)->GetType() != ET_SMALLENEMY )
					continue;
				
				this->GetTree()->Reset();

				Sphere* other = ((Sphere*)(((CBaseEntity*)(*iter))->GetCollisionEntity()));
				Sphere* smEnemy = ((Sphere*)this->GetCollisionEntity());
				//Vec3f velocity;
				Vec3f OCPC;
				//Finds the vector between player's sphere and the static sphere
				glsSubtractVectors3(OCPC, this->GetRenderNode().GetPosition(), ((CBaseEntity*)(*iter))->GetRenderNode().GetPosition());
				glsNormalizeVector3(OCPC);
				float dist = (other->m_Radius + smEnemy->m_Radius) - glsGetDistance3(this->GetRenderNode().GetPosition(), ((CBaseEntity*)(*iter))->GetRenderNode().GetPosition());
				//glsScaleVector3(OCPC, dist * 0.5f);

				Vec3f up;
				glsCopyVector3(up, GetRenderNode().GetUpVector());
				glsVectorMultiply3(up, up, OCPC);
				
				glsSubtractVectors3(OCPC, OCPC, up);
				glsScaleVector3(OCPC, dist * 0.5f);


				// if the sumMove hasn't be set then just use the first vector
				if(!SmallCol)
				{
					glsCopyVector3(sumMove, OCPC);
					SmallCol = true;
				}
				else
				{
					// Otherwise find the largest part for each axis of the vector.
					for(int i = 0; i < 3; i++)
					{
						if(abs(OCPC[i]) > abs(sumMove[i]))
							sumMove[i] = OCPC[i];
					}
				}
			}
			break;
		case ET_BULLET_PLAYER:
			{
				if(this->GetState() == Enemy_Dying)
					continue;

				if(this->GetHealth() > 0)
					this->SetHealth( this->GetHealth() - YELLOW_DAMAGE );
				else
					continue;

				/*if(this->GetHealth() <= 0 && m_DeathTimer <= 4.9f)
					CMessageSystem::GetInstance()->SendMessageW(new CGiveExperienceMessage(EXP_SMALL_ENEMY,YELLOW));*/
				//std::cout << "Enemy Health: " << this->GetHealth() << "/" << 2 << std::endl;
				if(this->GetHealth() <= 0)
				{
					CMessageSystem::GetInstance()->SendMessageW(new CGiveExperienceMessage(EXP_SMALL_ENEMY,YELLOW));
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_METAL_EXPLOSION_DEATH);
				}

				CMessageSystem::GetInstance()->SendMessageW(new CCreateVFXMessage(EMITTER_ENEMY_DAMAGE,this,0.0f,0.0f,0.0f));
			}
			break;
		case ET_BLUESTRIKE:
			{
				if(this->GetHealth() > 0 && !((CBlueStrike*)(*iter))->GetIsCrate())
				{
					this->SetHealth( this->GetHealth() - 100 );
					if(this->GetHealth() <= 0)
						CMessageSystem::GetInstance()->SendMessageW(new CGiveExperienceMessage(EXP_SMALL_ENEMY,YELLOW));
				}
			}
			break;
		case ET_PLAYER:
			{
				if( this->GetState() == Enemy_Vulnerable )
				{
					int type = this->GetPowerupType();
					((CPlayer*)(*iter))->GetGun().Store( type );
					//((CPlayer*)(*iter))->SetIsLoaded(true);
					((CPlayer*)(*iter))->GetGun().Process();
					((CPlayer*)(*iter))->HasPulled();
					CMessageSystem::GetInstance()->SendMessage(new CCreateVFXMessage(EMITTER_POWERUP,*iter,0.0f,0.0f,0.0f));
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_GUN_LOAD_LVL3);
					Kill();
				}
				else if( this->GetPowerupType() == BLACK && this->GetState() != Enemy_Dying )
				{
					this->SetState( Enemy_Dying );
					m_DeathTimer = SMALL_ENEMY_TIMER_DEATH;
					m_ExploderBlinkTimer = SMALL_ENEMY_TIMER_DEATH;
				}
			}
			break;

		case ET_PULL:
			{
				if(this->GetState() == Enemy_Dying)
					continue;

				this->SetState( Enemy_Vulnerable );
				m_IsPulled = true;
			}
			break;
		case ET_PUSH:
			{
				if(this->GetState() == Enemy_Dying)
					continue;

				this->SetState( Enemy_Pushed );
				m_IsPushed = true;
				m_PushTimer = SMALL_ENEMY_TIMER_PUSHED;

				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_ENEMY_STUNNED);

				//glsCopyVector3(this->GetRenderNode().m_vForward, ((CPush*)(*iter))->GetPusher()->GetRenderNode().m_vForward);
				Vec3f newFor;
				glsSubtractVectors3(newFor, *((CPush*)(*iter))->GetPusher()->GetPosition(), this->GetRenderNode().GetPosition());
				glsNormalizeVector3(newFor);
				GetRenderNode().SetForwardVector( newFor );
			}
			break;
		case ET_EXPLOSION:
			{
				if(!((CExplosion*)(*iter))->GetIsEMP())
				{
					if(this->GetHealth() > 0)
					{
						AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_EXPLOSION_DAMAGE);
						if(((CProjectile*)(*iter))->GetIfConsumed() == true)
							this->SetHealth( this->GetHealth() - CONSUMED_DAMAGE );
						else
							this->SetHealth( this->GetHealth() - EXPLOSION_DAMAGE );
						// TODO make sure that this isnt an explosion from barrel...
						if(this->GetHealth() <= 0 && ((CExplosion*)(*iter))->GetOwner() == ET_MORTAR)
							CMessageSystem::GetInstance()->SendMessageW(new CGiveExperienceMessage(EXP_SMALL_ENEMY,RED));
						
						CMessageSystem::GetInstance()->SendMessageW(new CCreateVFXMessage(EMITTER_PLAYER_DAMAGE,this,0.0f,0.0f,0.0f));
					}
					//// KNOCK BACK.
					//Vec3f toExplode;
					//glsSubtractVectors3(toExplode,this->GetRenderNode().GetPosition(),((CExplosion*)(*iter))->GetRenderNode().GetPosition());
					//glsNormalizeVector3(toExplode);
					//glsScaleVector3(toExplode,40.0f);
					//
					//glsLoadVector3(*(this->GetVelocity()),0.0f,0.0f,0.0f);
					//glsAddVectors3(*(this->GetVelocity()),*(this->GetVelocity()),toExplode);
				}
			}
			break;
		case ET_LASER:
			{
				if(m_SpawnTimer > 0.0f)
					continue;

				if(m_LaserTimer == 0.0f)
				{
					this->SetHealth( this->GetHealth() - BLUE_DAMAGE );
					m_LaserTimer = 0.1f;	

					//looks wierd when theyre hit with the laser with their dissolve effect so i removed the effect - Tom
					//CMessageSystem::GetInstance()->SendMessageW(new CCreateVFXMessage(EMITTER_ENEMY_DAMAGE,this,0.0f,0.0f,0.0f));
					
					if(GetHealth() <= 0.0f)
					{
						GetRenderNode().SetEnableBlending( true );
						GetRenderNode().SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[DISSOLVE] );
						GetRenderNode().SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[DISSOLVE] );
						GetRenderNode().SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[DISSOLVE] );
						GetRenderNode().SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[DISSOLVE] );
						GetRenderNode().SetTextureUnitHandle1( CAssetManager::GetInstance()->m_vTextureUnit1Handles[DISSOLVE] );
						GetRenderNode().SetDissolveFactorUniformHandle( CAssetManager::GetInstance()->m_vUniformDissolveFactorHandles[DISSOLVE] );
						GetRenderNode().SetSpecularTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_SHIELD).m_hTextureSpecular );
						SetLaserDeath(true);
						CMessageSystem::GetInstance()->SendMessageW(new CGiveExperienceMessage(EXP_SMALL_ENEMY,BLUE));
						AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_ENERGY_DEATH);

					}
				}
				break;
			}
			break;
		}
	}

	if(SmallCol)
	{
		Vec3f pos;
		glsAddVectors3(pos, this->GetRenderNode().GetPosition(), sumMove);
		GetRenderNode().SetPosition( pos );
		glsCopyVector3(*this->GetPosition(), pos);
	}

	Vec3f moon = { 0.0f, 75.0f, 0.0f };
	if(GetState() != Enemy_Dying)
		Adjust(moon);

	/*if(m_DeathTimer == m_YellowTimer && hitbyYellow)
	{
		CMessageSystem::GetInstance()->SendMessageW(new CGiveExperienceMessage(EXP_SMALL_ENEMY,YELLOW));
		hitbyYellow = false;
	}*/

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Orients the small enemy to face the passed in target.
Parameters:
[in] _target - the target to orient towards.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CSmallEnemy::TurnTo( Vec3f _target )
{
	Vec3f nFwd, nRight;
	glsSubtractVectors3( nFwd, this->GetRenderNode().GetPosition(), _target );
	glsNormalizeVector3( nFwd );

	glsCrossProduct3( nRight, nFwd, this->GetRenderNode().GetUpVector() ); 
	glsNormalizeVector3( nRight );
	GetRenderNode().SetRightVector( nRight );

	glsCrossProduct3( nFwd, nRight, this->GetRenderNode().GetUpVector() );
	glsNormalizeVector3( nFwd );
	GetRenderNode().SetForwardVector( nFwd );
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Updates the velocity in a way that allows the small enemy to move
		 across the spherical planetoid.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CSmallEnemy::UpdateVelocity( void )
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
