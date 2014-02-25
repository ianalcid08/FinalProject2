/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : MediumEnemy.cpp
Author : Evan Wright

Description : The medium enemy that is the main projectile unit in the game. He fires 
			  mortars and plasma cannons.

Created :  07/16/2013
Modified : 08/14/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "../Header Files/stdafx.h"
#include "../Header Files/MediumEnemy.h"
#include "../Header Files/Explosion.h"

#include "../Header Files/AssetManager.h"
#include "../Header Files/MessageSystem.h"
#include "../Header Files/CollisionEntity.h"
#include "../Header Files/CollisionLib.h"


//Sound includes...........................................
#include "../../Includes/Sound/Source/Wwise_IDs.h"
//#include "../../Includes/Sound/Source/Entity3D.h"
#include "../../Includes/Sound/Source/AudioSystemWwise.h"
//.........................................................

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Default constructor
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
CMediumEnemy::CMediumEnemy( void )
{
	this->SetType(ET_MEDIUMENEMY);

	GetRenderNode().SetUpVector( 0.0f, 1.0f, 0.0f );
	GetRenderNode().SetRightVector( 1.0f, 0.0f, 0.0f );
	GetRenderNode().SetForwardVector( 0.0f, 0.0f, 1.0f );

	// For movement...
	glsCopyVector3(m_MoveUp,		this->GetRenderNode().GetUpVector());
	glsCopyVector3(m_MoveRight,		this->GetRenderNode().GetRightVector());
	glsCopyVector3(m_MoveForward,	this->GetRenderNode().GetForwardVector());

	// COLLISION DATA
	Vec3f pos = { 0.0f, 0.0f, 0.0f };
	this->SetCollisionEntity(new Sphere(3.0f, pos));
	this->SetHealth(MAX_HP_MED_ENEMY);
	this->SetExperienceValue(EXP_MEDIUM_ENEMY);

	Reset();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Initializes all data for render node.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CMediumEnemy::Init(void)
{
	//GetRenderNode().hUniformMVP = CAssetManager::GetInstance()->m_vUniformMVPHandles[TEX_POINT_LIGHT];
	//GetRenderNode().hUniformMV = CAssetManager::GetInstance()->m_vUniformMVHandles[TEX_POINT_LIGHT];
	//GetRenderNode().hUniformNormal = CAssetManager::GetInstance()->m_vUniformNormalHandles[TEX_POINT_LIGHT];
	//GetRenderNode().hUniformColor = CAssetManager::GetInstance()->m_vUniformColorHandles[TEX_POINT_LIGHT];
	//GetRenderNode().hTextureUnit0 = CAssetManager::GetInstance()->m_vTextureUnit0Handles[TEX_POINT_LIGHT];
	//GetRenderNode().hTextureUnit1 = CAssetManager::GetInstance()->m_vTextureUnit1Handles[TEX_POINT_LIGHT];
	//GetRenderNode().hTextureUnit2 = CAssetManager::GetInstance()->m_vTextureUnit2Handles[TEX_POINT_LIGHT];
	//GetRenderNode().hUniformLightPos[0] = CAssetManager::GetInstance()->m_vUniformLightPosHandles[0][TEX_POINT_LIGHT];

	GetRenderNode().SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[MEDIUM_ENEMY_SHAPE_TB] );
	GetRenderNode().SetDiffuseTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_MEDIUMENEMY).m_DiffuseHandle );
	GetRenderNode().SetSpecularTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_MEDIUMENEMY).m_hTextureSpecular );
	GetRenderNode().SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[ANIMATED_ONLY] );
	GetRenderNode().SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[ANIMATED_ONLY] );
	GetRenderNode().SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[ANIMATED_ONLY] );
	GetRenderNode().SetSkinningMatrixUniformHandle( CAssetManager::GetInstance()->m_vUniformJointHandles[ANIMATED_ONLY] );
	GetRenderNode().SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[ANIMATED_ONLY] );
	GetRenderNode().SetColor(1.0f, 1.0f, 1.0f, 1.0f);

	this->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_MEDIUMENEMY, MEDIUM_MOVE_FORWARD), 0.1f);
	GetRenderNode().SetDissolveFactor( 1.0f );
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Kills the enemy... Sets all the necessary variables to be 'reborn' later.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CMediumEnemy::Kill(void)
{
	this->SetIsActive(false);
	this->SetIsAlive(false);
	this->SetIsDying(true);

	Reset();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Resets all the data for the medium enemy, minus the orientation/position/rendernode.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CMediumEnemy::Reset(void)
{
	m_StunTimer		= 0.0f;
	m_PushTimer		= 0.0f;
	m_LaserTimer	= 0.0f;
	m_DeathTimer	= 0.0f;
	m_HurtTimer		= 0.0f;
	m_SpawnTimer	= 2.5f;

	m_PowerUp = false;
	m_PowerUpTimer = 0.0f;

	this->SetMoveSpeed(20);
	this->SetState(Enemy_Charging);
	this->SetHealth(MAX_HP_MED_ENEMY);

	GetRenderNode().SetEnableBlending( false );
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Destructor
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CMediumEnemy::Release( void )
{
	CEnemy::Release();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Updates the medium enemy!
Parameters:
[in] _elapsedTime - the time passed since the last frame.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool CMediumEnemy::Update( float _elapsedTime )
{
#if _DEBUG
	if(this->GetIsPaused())
	{
		this->GetAnimationManager()->Update(_elapsedTime, &this->GetRenderNode());
		return true;
	}
#endif

	if(m_SpawnTimer > 0.0f)
		m_SpawnTimer -= _elapsedTime;

	Vec4f white = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vec4f black = { 0.2f, 0.2f, 0.2f, 1.0f };
	// If they are preparing an attack.
	if( this->GetState() == Enemy_Charging )
	{
		if(!m_PowerUp && m_PowerUpTimer <= 0.0f)
		{
			CMessageSystem::GetInstance()->SendMessage(new CCreateVFXMessage(EMITTER_POWERUP,this,0.0f,0.0f,0.0f));
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_MEDIUM_MORTAR_CHARGE);
//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_MEDIUM_MORTAR_CHARGE);
			m_PowerUpTimer = 6.0f;
			m_PowerUp = true;
		}


		static float tell_blink = 0.2f;
		tell_blink += _elapsedTime;
		if(tell_blink >= 0.2f)
		{
			if(this->GetRenderNode().GetColor()[0] == white[0])
				this->GetRenderNode().SetColor(black);
			else
				this->GetRenderNode().SetColor(white);

			tell_blink = 0.0f;
		}
	}
	else if(this->GetRenderNode().GetColor()[0] != white[0])
		this->GetRenderNode().SetColor(white);

	if(m_PowerUp)
	{
		m_PowerUpTimer -= _elapsedTime;

		if(m_PowerUpTimer <= 0.0f)
		{
			m_PowerUp = false;
			//AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::);
		}
	}

	// If they are dying.
	if( this->GetState() == Enemy_Dying )
	{
		// Move the enemy into the planet, so it doesn't
		// pop out of the game.
		if(!GetLaserDeath())
		{
			Vec3f nPos, move;
			glsCopyVector3(move,GetRenderNode().GetUpVector());
			glsScaleVector3(move, 10.5f * _elapsedTime);
			glsAddVectors3(nPos,GetRenderNode().GetPosition(),move);
			GetRenderNode().SetPosition( nPos );
		
			// If death timer has just started:
			if( m_DeathTimer == MEDIUM_ENEMY_TIMER_DEATH )
			{
				CMessageSystem::GetInstance()->SendMessageW(new CCreateVFXMessage(EMITTER_EXPLOSION,this,0.0f,0.0f,0.0f));
			}

			// Decrement the death timer.dd
			m_DeathTimer -= _elapsedTime;

			// Check if the timer is finished.
			// If so... KILL ZE ENEMY!
			if(m_DeathTimer <= 0.0f)
				Kill();

			if(m_HurtTimer > 0.0f)
			{
				this->GetRenderNode().SetColor(white);
				m_HurtTimer = 0.0f;
			}
		}
		else
		{
			GetRenderNode().SetDissolveFactor( GetRenderNode().GetDissolveFactor() - _elapsedTime );
			if(GetRenderNode().GetDissolveFactor() <= 0.0f)
			{
				this->SetIsActive(false);
				this->SetIsAlive(false);
				SetLaserDeath(false);
				//Init();
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

	Vec4f red = {1.0f, 0.0f, 0.0f, 1.0f};
	if( m_HurtTimer > 0.0f )
	{
		GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_MEDIUMENEMY, MEDIUM_HURT), 0.4f);
		if(this->GetRenderNode().GetColor()[1] == white[1])
			this->GetRenderNode().SetColor(red);

		m_HurtTimer -= _elapsedTime;
		if( m_HurtTimer <= 0.0f )
			this->GetRenderNode().SetColor(white);
	}


	// Orient the movement vector
	glsCopyVector3(m_MoveUp, this->GetRenderNode().GetUpVector());
	glsCrossProduct3( m_MoveForward, m_MoveRight, m_MoveUp );
	glsNormalizeVector3( m_MoveForward );
	glsCrossProduct3( m_MoveRight, m_MoveUp, m_MoveForward );
	glsNormalizeVector3( m_MoveRight );

	UpdateVelocity();
	SwitchAnimation();
	CEnemy::Update(_elapsedTime);

	// Death stuff:
	if(this->GetHealth() <= 0 && this->GetState() != Enemy_Dying)
	{
		this->SetState(Enemy_Dying);
		m_DeathTimer = MEDIUM_ENEMY_TIMER_DEATH;
	}

	return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Updates the velocity in a way that allows the small enemy to move
		 across the spherical planetoid.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CMediumEnemy::UpdateVelocity( void )
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Handles the collision reaction.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CMediumEnemy::HandleReaction( void )
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
				if(this->GetState() == Enemy_Charging || m_SpawnTimer > 0.0f)
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
		case ET_WORLD:
			{
				if(this->GetState() == Enemy_Dying)
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
		case ET_BULLET_PLAYER:
			{
				if(this->GetHealth() > 0)
					this->SetHealth( this->GetHealth() - YELLOW_DAMAGE );
				else
					continue;
				if(this->GetHealth() <= 0)
				{
					CMessageSystem::GetInstance()->SendMessageW(new CGiveExperienceMessage(EXP_MEDIUM_ENEMY,YELLOW));
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_METAL_EXPLOSION_DEATH);
				}

				CMessageSystem::GetInstance()->SendMessageW(new CCreateVFXMessage(EMITTER_PLAYER_DAMAGE,this,0.0f,0.0f,0.0f));
				//AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS:);

//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_PUNCHES);


				m_HurtTimer = MEDIUM_ENEMY_TIMER_HURT;
			}
			break;
		case ET_BLUESTRIKE:
			{
				if(this->GetHealth() > 0)
					this->SetHealth( this->GetHealth() - 100 );
				else
					CMessageSystem::GetInstance()->SendMessageW(new CGiveExperienceMessage(EXP_MEDIUM_ENEMY,YELLOW));
			}
			break;
		case ET_EXPLOSION:
			{
				if(!((CExplosion*)(*iter))->GetIsEMP())
				{
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_EXPLOSION_DAMAGE);
					if(GetState() == Enemy_Dying)
						continue;

					this->SetHealth( this->GetHealth() - EXPLOSION_DAMAGE );
					m_HurtTimer = MEDIUM_ENEMY_TIMER_HURT;

					if(this->GetHealth() <= 0 && ((CExplosion*)(*iter))->GetOwner() == ET_MORTAR )
						CMessageSystem::GetInstance()->SendMessageW(new CGiveExperienceMessage(EXP_MEDIUM_ENEMY,RED));

					CMessageSystem::GetInstance()->SendMessageW(new CCreateVFXMessage(EMITTER_PLAYER_DAMAGE,this,0.0f,0.0f,0.0f));

					// KNOCK BACK.
					/*Vec3f toExplode;
					glsSubtractVectors3(toExplode,this->GetRenderNode().GetPosition(),((CExplosion*)(*iter))->GetRenderNode().GetPosition());
					glsNormalizeVector3(toExplode);
					glsScaleVector3(toExplode,40.0f);
					
					glsAddVectors3(*(this->GetVelocity()),*(this->GetVelocity()),toExplode);*/
				}
			}
			break;
		case ET_LASER:
			{
				if(m_LaserTimer == 0.0f)
				{
					this->SetHealth( this->GetHealth() - BLUE_DAMAGE );
					m_LaserTimer = 0.1f;
					m_HurtTimer = MEDIUM_ENEMY_TIMER_HURT;

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
						CMessageSystem::GetInstance()->SendMessageW(new CGiveExperienceMessage(EXP_MEDIUM_ENEMY,BLUE));

						CMessageSystem::GetInstance()->SendMessageW(new CCreateVFXMessage(EMITTER_PLAYER_DAMAGE,this,0.0f,0.0f,0.0f));
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

void CMediumEnemy::SwitchAnimation(void)
{
	CAnimationManager* pAM = this->GetAnimationManager();

	switch(this->GetState())
	{
	case Enemy_Attacking:
		
		break;

	case Enemy_Charging:
		this->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_MEDIUMENEMY, MEDIUM_FIREMORTAR), 0.4f);
		break;

	default:

		break;
	}
}
