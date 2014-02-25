/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : LargeEnemy.cpp
Author : Evan Wright

Description : The large enemy will be the primary melee unit in the game. It will fire
			  a tractor beam at the player, that pulls the the player towards it. When
			  the player gets close enough the large enemy strikes and does massive 
			  damage!

Created :  07/16/2013
Modified : 07/16/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "../Header Files/stdafx.h"
#include "../Header Files/LargeEnemy.h"
#include "../Header Files/Explosion.h"
#include "../Header Files/Shield.h"

#include "../Header Files/AssetManager.h"
#include "../Header Files/MessageSystem.h"
#include "../Header Files/CollisionEntity.h"
#include "../Header Files/CollisionLib.h"

//SoundManager includes....................................
#include "../../Includes/Sound/Source/Wwise_IDs.h"
#include "../../Includes/Sound/Source/AudioSystemWwise.h"
//.........................................................

CLargeEnemy::CLargeEnemy( void )
{
	this->SetType(ET_LARGEENEMY);

	GetRenderNode().SetUpVector( 0.0f, 1.0f, 0.0f );
	GetRenderNode().SetRightVector( 1.0f, 0.0f, 0.0f );
	GetRenderNode().SetForwardVector( 0.0f, 0.0f, 1.0f );

	// For movement...
	glsCopyVector3(m_MoveUp,		this->GetRenderNode().GetUpVector());
	glsCopyVector3(m_MoveRight,		this->GetRenderNode().GetRightVector());
	glsCopyVector3(m_MoveForward,	this->GetRenderNode().GetForwardVector());

	// COLLISION DATA
	Vec3f pos = { 0.0f, 0.0f, 0.0f };
	this->SetCollisionEntity(new Sphere(4.0f, pos));
	this->SetHealth(MAX_HP_LARGE_ENEMY);

	m_Shield = nullptr;
	this->SetExperienceValue(EXP_LARGE_ENEMY);

	Reset();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Initializes all data for render node.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CLargeEnemy::Init(void)
{
	GetRenderNode().SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[LARGE_ENEMY_SHAPE_TB] );
	GetRenderNode().SetDiffuseTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_LARGEENEMY).m_DiffuseHandle );
	GetRenderNode().SetSpecularTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_LARGEENEMY).m_hTextureSpecular );

	GetRenderNode().SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[ANIMATED_ONLY] );
	GetRenderNode().SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[ANIMATED_ONLY] );
	GetRenderNode().SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[ANIMATED_ONLY] );
	GetRenderNode().SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[ANIMATED_ONLY] );
	GetRenderNode().SetSkinningMatrixUniformHandle( CAssetManager::GetInstance()->m_vUniformJointHandles[ANIMATED_ONLY] );

	SetLaserDeath(false);
	GetRenderNode().SetDissolveFactor( 1.0f );

	GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_LARGEENEMY, LARGE_IDLE), 0.1f);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Kills the enemy... Sets all the necessary variables to be 'reborn' later.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CLargeEnemy::Kill(void)
{
	this->SetIsActive(false);
	this->SetIsAlive(false);
	this->SetIsDying(true);
	

	if(this->m_Shield != nullptr)
	{
		((CShield*)m_Shield)->ResetShield();
		m_Shield->SetIsActive(false);
		m_Shield->SetIsAlive(false);
		m_Shield = nullptr;
	}

	CMessageSystem::GetInstance()->SendMessage(new CCreateExplosionMessage(this,ET_LARGEENEMY,true,4.0f));

	Reset();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Resets all the data for the medium enemy, minus the orientation/position/rendernode.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CLargeEnemy::Reset(void)
{
	m_StunTimer			= 0.0f;
	m_PushTimer			= 0.0f;
	m_LaserTimer		= 0.0f;
	m_DeathTimer		= 0.0f;
	m_HurtTimer			= 0.0f;
	m_JustSpawnedTimer	= 2.5f;
	m_NoShield			= false;

	this->SetMoveSpeed(20);
	this->SetState(Enemy_Charging);
	this->SetHealth(MAX_HP_LARGE_ENEMY);

	GetRenderNode().SetEnableBlending( false );
}

// IEntity interface:
void CLargeEnemy::Release( void )
{

}

bool CLargeEnemy::Update( float _elapsedTime )
{
	if(GetIsPaused())
	{
		this->GetAnimationManager()->Update(_elapsedTime, &GetRenderNode());
		return true;
	}

	if(m_JustSpawnedTimer > 0.0f)
		m_JustSpawnedTimer-=_elapsedTime;
	if(this->m_Shield != nullptr)
	{
		if(this->m_Shield->GetIsActive() && this->m_Shield->GetIsAlive())
		{
			m_Shield->GetRenderNode().SetPosition( GetRenderNode().GetPosition() );
			m_Shield->GetRenderNode().SetForwardVector(0.0f,0.0f,-0.8f);
			m_Shield->GetRenderNode().SetRightVector(0.8f,0.0f,0.0f);
			m_Shield->GetRenderNode().SetUpVector(0.0f,0.8f,0.0f);

			((Sphere*)m_Shield->GetCollisionEntity())->m_Radius = 20.0f;
			m_NoShield = false;
		}
		else
			m_NoShield = true;
	}

	if( GetState() == Enemy_Charging )
		GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_LARGEENEMY, LARGE_ALPHA), 0.3f);
	else if( GetState() == Enemy_Dying )
		GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_LARGEENEMY, LARGE_DEATH), 0.3f);
	else
		GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_LARGEENEMY, LARGE_IDLE), 0.3f);

	// If they are preparing an attack.
	if( this->GetState() == Enemy_Charging )
	{
		static float tell_blink = 0.2f;
		tell_blink += _elapsedTime;
		if(tell_blink >= 0.2f)
		{
			if(this->GetRenderNode().GetColor()[0] == 0.0f)
				this->GetRenderNode().SetColor(1.0f, 1.0f, 1.0f, 1.0f);
			else
				this->GetRenderNode().SetColor(0.0f, 0.0f, 0.0f, 1.0f);

			tell_blink = 0.0f;
		}
	}
	else if(this->GetRenderNode().GetColor()[0] == 0.0f)
		this->GetRenderNode().SetColor(1.0f, 1.0f, 1.0f, 1.0f);

	// If they are dying.
	if( this->GetState() == Enemy_Dying )
	{
		if(!GetLaserDeath())
		{
			// Move the enemy into the planet, so it doesn't
			// pop out of the game.
			/*Vec3f nPos, move;
			glsCopyVector3(move,GetRenderNode().GetUpVector());
			glsScaleVector3(move, 5.5f * _elapsedTime);
			glsAddVectors3(nPos,GetRenderNode().GetPosition(),move);
			GetRenderNode().SetPosition( nPos );*/


			// Decrement the death timer.
			m_DeathTimer -= _elapsedTime;

			// Check if the timer is finished.
			// If so... KILL ZE ENEMY!
			if(m_DeathTimer <= 0.0f)
			{
				Kill();
				CMessageSystem::GetInstance()->SendMessageW(new CCreateVFXMessage(EMITTER_EXPLOSION,this,0.0f,0.0f,0.0f));
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_ENERGY_DEATH);
			}

			this->GetRenderNode().SetColor(1.0f, 1.0f, 1.0f);
		}
		else
		{
			GetRenderNode().SetDissolveFactor( GetRenderNode().GetDissolveFactor() - _elapsedTime );
			if(GetRenderNode().GetDissolveFactor() <= 0.0f)
			{
				this->SetIsActive(false);
				this->SetIsAlive(false);
				SetLaserDeath(false);
				CMessageSystem::GetInstance()->SendMessageW(new CCreateVFXMessage(EMITTER_EXPLOSION,this,0.0f,0.0f,0.0f));
				
				//Init();
			}
		}

		GetAnimationManager()->Update(_elapsedTime, &GetRenderNode());
		return true;
	}

	// Update laser invulnerablity timer
	if( m_LaserTimer > 0.0f )
	{
		m_LaserTimer -= _elapsedTime;
		if( m_LaserTimer < 0.0f )
			m_LaserTimer = 0.0f;
	}

	if( m_HurtTimer > 0.0f )
	{
		if(this->GetRenderNode().GetColor()[1] == 1.0f)
			this->GetRenderNode().SetColor(1.0f, 0.3f, 0.3f);

		m_HurtTimer -= _elapsedTime;
		if( m_HurtTimer <= 0.0f )
			this->GetRenderNode().SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	}


	// Orient the movement vector
	glsCopyVector3(m_MoveUp, this->GetRenderNode().GetUpVector());
	glsCrossProduct3( m_MoveForward, m_MoveRight, m_MoveUp );
	glsNormalizeVector3( m_MoveForward );
	glsCrossProduct3( m_MoveRight, m_MoveUp, m_MoveForward );
	glsNormalizeVector3( m_MoveRight );

	UpdateVelocity();
	CEnemy::Update(_elapsedTime);

	// Death stuff:
	if(this->GetHealth() <= 0 && this->GetState() != Enemy_Dying)
	{
		this->SetState(Enemy_Dying);
		m_DeathTimer = 5.0f;
	}

	return true;
}

// Math Helpers
void CLargeEnemy::UpdateVelocity( void )
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

void CLargeEnemy::TurnTo( Vec3f _target )
{

}

void CLargeEnemy::TakeDamage(int _amount)
{
	if(m_NoShield)
	{
		if(this->GetHealth() > 0)
			this->SetHealth( this->GetHealth() - _amount );
		m_HurtTimer = MEDIUM_ENEMY_TIMER_HURT;
	}
}

void CLargeEnemy::HandleReaction( void )
{
	bool SmallCol = false;
	Vec3f sumMove = { 0.0f, 0.0f, 0.0f };
	std::vector<IEntity*> Col = this->GetCollisions();
	for(std::vector<IEntity*>::iterator iter = Col.begin(); iter != Col.end(); ++iter)
	{
		switch((*iter)->GetType())
		{
		case ET_CRATE:
		case ET_REDBARREL:
		case ET_SMALLENEMY:
		case ET_MEDIUMENEMY:
		case ET_SHIELD:
			{
				if(this->GetState() == Enemy_Charging || m_JustSpawnedTimer > 0.0f || (*iter) == m_Shield)
					continue;

				Sphere* other = ((Sphere*)(((CBaseEntity*)(*iter))->GetCollisionEntity()));
				Sphere* smEnemy = ((Sphere*)this->GetCollisionEntity());
				//Vec3f velocity;
				Vec3f OCPC;
				//Finds the vector between player's sphere and the static sphere
				glsSubtractVectors3(OCPC, this->GetRenderNode().GetPosition(), ((CBaseEntity*)(*iter))->GetRenderNode().GetPosition());
				glsNormalizeVector3(OCPC);
				float dist = (other->m_Radius + smEnemy->m_Radius) - glsGetDistance3(this->GetRenderNode().GetPosition(), ((CBaseEntity*)(*iter))->GetRenderNode().GetPosition());
				glsScaleVector3(OCPC, dist);
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
		//case ET_WORLD:
		//	{
		//		if(this->GetState() == Enemy_Dying)
		//			continue;

		//		Sphere* other = ((Sphere*)(((CBaseEntity*)(*iter))->GetCollisionEntity()));
		//		Sphere* smEnemy = ((Sphere*)this->GetCollisionEntity());
		//		//Vec3f velocity;
		//		Vec3f OCPC;
		//		//Finds the vector between player's sphere and the static sphere
		//		glsSubtractVectors3(OCPC, this->GetRenderNode().GetPosition(), ((CBaseEntity*)(*iter))->GetRenderNode().GetPosition());
		//		glsNormalizeVector3(OCPC);
		//		float dist = (other->m_Radius + smEnemy->m_Radius) - glsGetDistance3(this->GetRenderNode().GetPosition(), ((CBaseEntity*)(*iter))->GetRenderNode().GetPosition());
		//		glsScaleVector3(OCPC, dist);
		//		// if the sumMove hasn't be set then just use the first vector
		//		if(!SmallCol)
		//		{
		//			glsCopyVector3(sumMove, OCPC);
		//			SmallCol = true;
		//		}
		//		else
		//		{
		//			// Otherwise find the largest part for each axis of the vector.
		//			for(int i = 0; i < 3; i++)
		//			{
		//				if(abs(OCPC[i]) > abs(sumMove[i]))
		//					sumMove[i] = OCPC[i];
		//			}
		//		}
		//	}
		//	break;
		case ET_BULLET_PLAYER:
			{
				if(this->GetHealth() > 0)
					TakeDamage(YELLOW_DAMAGE);
				else
					continue;
				if(this->GetHealth() <= 0)
					CMessageSystem::GetInstance()->SendMessageW(new CGiveExperienceMessage(EXP_LARGE_ENEMY,YELLOW));
			}
			break;
		case ET_BLUESTRIKE:
			{
				TakeDamage(100);
				if(this->GetHealth() <= 0)
					CMessageSystem::GetInstance()->SendMessageW(new CGiveExperienceMessage(EXP_LARGE_ENEMY,YELLOW));
			}
			break;
		case ET_EXPLOSION:
			{
				if(!((CExplosion*)(*iter))->GetIsEMP())
				{
					TakeDamage(EXPLOSION_DAMAGE);
					if(this->GetHealth() <= 0 && ((CExplosion*)(*iter))->GetOwner() == ET_MORTAR)
						CMessageSystem::GetInstance()->SendMessageW(new CGiveExperienceMessage(EXP_LARGE_ENEMY,RED));

					//if(this->GetHealth() > 0.0f)
					{
						// KNOCK BACK.
						/*Vec3f toExplode;
						glsSubtractVectors3(toExplode,this->GetRenderNode().GetPosition(),((CExplosion*)(*iter))->GetRenderNode().GetPosition());
						glsNormalizeVector3(toExplode);
						glsScaleVector3(toExplode,40.0f);
						
						glsAddVectors3(*(this->GetVelocity()),*(this->GetVelocity()),toExplode);*/
					}
				}
			}
			break;
		case ET_LASER:
			{
				if(m_LaserTimer == 0.0f)
				{
					TakeDamage(BLUE_DAMAGE);
					m_LaserTimer = 0.1f;

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
						CMessageSystem::GetInstance()->SendMessageW(new CGiveExperienceMessage(EXP_LARGE_ENEMY,BLUE));
					}
				}
				break;
			}
			break;
		}
	}

	if(SmallCol)
	{
		Vec3f pos, relativePos, vertical, world = { 0.0f, 75.0f, 0.0f };
		float offset;
		glsAddVectors3(pos, this->GetRenderNode().GetPosition(), sumMove);
		glsSubtractVectors3( relativePos, pos, world );
		offset = glsGetVectorLength3( relativePos ) - 75.0f;
		glsCopyVector3( vertical, GetRenderNode().GetUpVector() );
		glsScaleVector3( vertical, offset );
		glsAddVectors3( pos, pos, vertical );
		glsCopyVector3(*this->GetPosition(), pos);
		GetRenderNode().SetPosition( pos );
	}
}
