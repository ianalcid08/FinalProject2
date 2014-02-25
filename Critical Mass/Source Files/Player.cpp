/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : Player.cpp
Author : Evan Wright

Description : The player! Will be central game win and lose decider! Is controlled
			  by the user, can die, collide and render. 

Created :  07/16/2013
Modified : 08/12/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "../Header Files/stdafx.h"
#include "../Header Files/Player.h"
#include "../Header Files/Definitions.h"
#include "../Header Files/glsMath.h"
#include "../Header Files/CollisionEntity.h"
#include "../Header Files/RenderNode.h"
#include "../Header Files/SmallEnemy.h"
#include "../Header Files/Explosion.h"
#include "../Header Files/CollisionLib.h"
#include "../Header Files/AIManager.h"
#include "../Header Files/Crate.h"
//SoundManager includes....................................
#include "../../Includes/Sound/Source/Wwise_IDs.h"
#include "../../Includes/Sound/Source/AudioSystemWwise.h"
//.........................................................
#include "../Header Files/AssetManager.h"
#include "../Header Files/MessageSystem.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Default constructor
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
CPlayer::CPlayer( void )
{
	// Set type to player.
	this->SetType(ET_PLAYER);

	// Set gun to empty.
	/*m_CurrGun = GS_EMPTY;*/
	this->SetIsDying(false);

	// default orientation.
	GetRenderNode().SetUpVector( 0.0f, 1.0f, 0.0f );
	GetRenderNode().SetRightVector( 1.0f, 0.0f, 0.0f );
	GetRenderNode().SetForwardVector( 0.0f, 0.0f, 1.0f );

	// Default velocity
	glsLoadVector3( m_UpVel,		0.0f, 0.0f, 0.0f );
	glsLoadVector3( m_ForwardVel,	0.0f, 0.0f, 0.0f );
	glsLoadVector3( m_RightVel,		0.0f, 0.0f, 0.0f );

	// For movement...
	glsCopyVector3(m_MoveUp,		this->GetRenderNode().GetUpVector());
	glsCopyVector3(m_MoveRight,		this->GetRenderNode().GetRightVector());
	glsCopyVector3(m_MoveForward,	this->GetRenderNode().GetForwardVector());

	m_MoveSpeed = 0.0015f;
	m_JumpTimer = 0.0f;
	m_CanJump = true;
	m_HealthPickup = 0.0f;

	Vec3f pos = { 0.0f, 0.0f, 0.0f };
	this->SetCollisionEntity(new Sphere(2.0f, pos));

	/*m_PlayerAnim.LoadAnimFromFile("Assets/Models/TestAnim.bin");
	m_PlayerProcess.SetAnimation(&m_PlayerAnim);*/

	this->GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_IDLE), 0.1f);

	m_PullTimer = 0.0f;
	m_PushTimer = 0.0f;

	m_PosXOffset = 0.0f;
	m_PosYOffset = 0.0f;

	m_Lives = MAX_LIVES_PLAYER;
	this->SetHealth(MAX_HP_PLAYER);
	this->SetInvulnerableTime(0.0f);

	m_DeathTimer = 0.0f;
	m_DissolveTimer = 0.0f;
	m_isLoaded = false;
	m_ResetWave = false;
	m_StopAnimSwitching = false;
	m_LowHPTimer = 0.0f;
	m_KillScreen = false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: destructor
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CPlayer::Release( void )
{
	CLivingEntity::Release();
}
void CPlayer::Init()
{
	GetRenderNode().SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[PLAYER_SHAPE_TB] );
	GetRenderNode().SetMesh( &CAssetManager::GetInstance()->m_playerMesh );
	GetRenderNode().SetDiffuseTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_PLAYER).m_DiffuseHandle );
	GetRenderNode().SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[ANIMATED_ONLY] );
	GetRenderNode().SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[ANIMATED_ONLY] );
	GetRenderNode().SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[ANIMATED_ONLY] );
	GetRenderNode().SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[ANIMATED_ONLY] );
	GetRenderNode().SetSkinningMatrixUniformHandle( CAssetManager::GetInstance()->m_vUniformJointHandles[ANIMATED_ONLY] );
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Updates the player!
Parameters:
[in] _elapsedTime - the time passed since the last frame.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool CPlayer::Update( float _elapsedTime )
{
	static bool once = false;
	// Timer for death animation.
	if( m_DeathTimer > 0.0f )
	{
		m_DeathTimer -= _elapsedTime;
		
		m_StopAnimSwitching = true;
		if( GetLives() <= 1 )
		{
				
			if( !once )
			{
				GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_LOSE), 0.5f);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_TRACK_04);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_TRACK_03);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_TRACK_02);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_TRACK_01);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_MX_TRACK_02);
				SetLives(0);
				once = true;
			}
		}
		else
		{
			GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_DEATH), 0.3f);
		}
			
		if(m_DeathTimer <= 0.0f)
		{
			SetIsDying(true);
			SetKillScreen(true);
			once = false;

			if( GetLives() == 0 )
			{
				m_DeathTimer = 0.0f;
				return true;
			}

			m_ResetWave = true;
			m_DeathTimer = 0.0f;
		}
		GetAnimationManager()->Update(_elapsedTime, &GetRenderNode());
		return true;
	}

	if(this->GetHealth() <= 25)
	{
		m_LowHPTimer += _elapsedTime;
		if(m_LowHPTimer > 1.5f)
		{
			m_LowHPTimer = 0.0f;
//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_BEEP);
		}
	}

	if(this->m_HealthPickup >= 0.0f)
	{
		static bool healthpickupcolor = false;
		if(healthpickupcolor)
		{
			//GetRenderNode().hTextureDiffuse = CAssetManager::GetInstance()->GetObjData(ET_PLAYER).m_DiffuseHandle;
			GetRenderNode().SetDiffuseTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_SMALLENEMY).m_DiffuseHandle );
		}
		else
		{
			GetRenderNode().SetDiffuseTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_PLAYER).m_DiffuseHandle );
		}
		m_HealthPickup -= HEALTH_PICKUP_BLINKING_TIME;
		healthpickupcolor = !healthpickupcolor;
	}
	else
	{
		GetRenderNode().SetDiffuseTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_PLAYER).m_DiffuseHandle );
	}

	if( GetIsDying() )
	{
		SetIsDying(false);
		SetKillScreen(false);
		//m_StopAnimSwitching = false;

		Vec3f direction;
		glsLoadVector3(direction, 0.0f, 0.0f, 0.0f);
		glsCopyVector3(*GetPosition(), direction);
		GetRenderNode().SetPosition( direction );

		ResetMatrix();
	}

	//	Vec3f direction;
	//	glsLoadVector3(direction, 0.0f, 0.0f, 0.0f);

	//	// Gets the two distances between the north and south pole (possibly in need of optimizations later).
	//	float SDist = glsGetDistance3(direction, GetRenderNode().m_vPosition);
	//	glsLoadVector3(direction, 0.0f, 150.0f, 0.0f);
	//	float NDist = glsGetDistance3(direction, GetRenderNode().m_vPosition);
	//	// finds the shortest one so it can travel to that one
	//	bool HeadinNorth = NDist < SDist ;

	//	// resets the vector if heading to the south
	//	if(!HeadinNorth)
	//		glsLoadVector3(direction, 0.0f, 0.0f, 0.0f);

	//	// gets the directional vector
	//	glsSubtractVectors3(direction, direction, GetRenderNode().m_vPosition);
	//	glsNormalizeVector3(direction);
	//	glsScaleVector3(direction, 30.0f * _elapsedTime);
	//	
	//	// moves the player by that direction by elapsedTime
	//	glsAddVectors3(GetRenderNode().m_vPosition, GetRenderNode().m_vPosition, direction);

	//	// if the distance is small enough the player stops moving
	//	//glsLoadVector3(direction, 0.0f, 0.0f, 0.0f);
	//	if( (HeadinNorth ? NDist : SDist) <= 0.5f )
	//	{
	//		SetIsDying(false);
	//		m_StopAnimSwitching = false;
	//		ResetMatrix();
	//		GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_IDLE), 0.01f);
	//	}

	//	// copying the player's new position%
	//	glsCopyVector3(*this->GetPosition(), GetRenderNode().m_vPosition);
	//	
	//	// remaking the player's up vector
	//	Vec3f world = { 0.0f, 75.0f, 0.0f };
	//	glsSubtractVectors3(direction, world, GetRenderNode().m_vPosition);

	//	glsNormalizeVector3(direction);
	//	glsCopyVector3(GetRenderNode().m_vUp, direction);

	//	glsCrossProduct3( this->GetRenderNode().m_vForward, this->GetRenderNode().m_vRight, this->GetRenderNode().m_vUp );
	//	glsNormalizeVector3( this->GetRenderNode().m_vForward );

	//	glsCrossProduct3( this->GetRenderNode().m_vRight, this->GetRenderNode().m_vUp, this->GetRenderNode().m_vForward );
	//	glsNormalizeVector3( this->GetRenderNode().m_vRight );

	//	// returing out so no other calcualtions are done.
	//	GetAnimationManager()->Update(_elapsedTime, &GetRenderNode());
	//	return true;
	//}

	if( m_PullTimer > 0.0f )
		m_PullTimer -= _elapsedTime;
	else if( m_PullTimer < 0.0f )
		m_PullTimer = 0.0f;

	if( m_PushTimer > 0.0f )
		m_PushTimer -= _elapsedTime;
	else if( m_PushTimer < 0.0f )
		m_PushTimer = 0.0f;

	m_Gun.AddTime(_elapsedTime);

	// Orient the movement
	glsCopyVector3(m_MoveUp, this->GetRenderNode().GetUpVector());
	glsCrossProduct3( m_MoveForward, m_MoveRight, m_MoveUp );
	glsNormalizeVector3( m_MoveForward );

	glsCrossProduct3( m_MoveRight, m_MoveUp, m_MoveForward );
	glsNormalizeVector3( m_MoveRight );

	// Update the velocities.
	UpdateVelocity();

	m_PosXOffset += (*this->GetVelocity())[0] * _elapsedTime;
	m_PosYOffset += (*this->GetVelocity())[1] * _elapsedTime;

	// Call parents update
	CLivingEntity::Update(_elapsedTime);

	// Update player specific data.
	// Player position	
	GetRenderNode().SetPosition( ((const float *)(*GetPosition())) );

	if( m_JumpTimer > 0.0f )
	{
		m_JumpTimer -= _elapsedTime;
	}
	else if( m_JumpTimer < 0.0f )
		m_JumpTimer = 0.0f;



	//////////////////////////////////////////////
	// Debug Code
	//////////////////////////////////////////////
	//if( GetAsyncKeyState('P') & 0x01 )
	//{
	//	std::cout << "HUD INFO:\n";
	//	std::cout << "HP : " << this->GetHealth() << '/' << MAX_HP_PLAYER << std::endl;
	//	std::cout << "Lives : " << this->GetLives() << std::endl;
	//	std::cout << "Ammo : " << this->GetGun().GetShotTotal() << std::endl;
	//	std::cout << "Curr Gun : " << this->GetGun().GetConsumed() << std::endl;
	//}

	//HAX
	//AUTH: Ryan
	//Desc: feedback for player hurt, replace with animation / effect later
	//Date: ????
	static float hurtTimer = 0.0f;
	if(m_Hurt)
	{
		GetRenderNode().SetColor(1.0f, 0.2f, 0.2f, 1.0f);
		hurtTimer += _elapsedTime;
		if(hurtTimer >= 0.5f)
		{
			//this->GetRenderNode().hTextureDiffuse = CAssetManager::GetInstance()->GetObjData(ET_PLAYER).m_DiffuseHandle;
			GetRenderNode().SetColor(1.0f, 1.0f, 1.0f, 1.0f);
			hurtTimer = 0.0f;
			m_Hurt = false;
		}
	}

	glsCopyVector3(((Sphere*)this->GetCollisionEntity())->m_Center, this->GetRenderNode().GetPosition());
	return true;
}

void CPlayer::ResetMatrix(void)
{
	// default orientation.
	GetRenderNode().SetUpVector( 0.0f, 1.0f, 0.0f );
	GetRenderNode().SetRightVector( 1.0f, 0.0f, 0.0f );
	GetRenderNode().SetForwardVector( 0.0f, 0.0f, 1.0f );

	// Default velocity
	glsLoadVector3( m_UpVel,		0.0f, 0.0f, 0.0f );
	glsLoadVector3( m_ForwardVel,	0.0f, 0.0f, 0.0f );
	glsLoadVector3( m_RightVel,		0.0f, 0.0f, 0.0f );

	// For movement...
	glsCopyVector3(m_MoveUp,		this->GetRenderNode().GetUpVector());
	glsCopyVector3(m_MoveRight,		this->GetRenderNode().GetRightVector());
	glsCopyVector3(m_MoveForward,	this->GetRenderNode().GetRightVector());
}

void CPlayer::UpdateVelocity( void )
{
	Vec3f totalVel;
	glsLoadVector3( totalVel, 0.0f, 0.0f, 0.0f );
	glsAddVectors3( totalVel, totalVel, m_ForwardVel );
	glsAddVectors3( totalVel, totalVel, m_RightVel );
	if(glsGetVectorLength3(totalVel) != 0.0f)
		glsNormalizeVector3(totalVel);
	glsScaleVector3(totalVel, 20.0f);
	this->SetVelocity(totalVel);
	glsLoadVector3( m_ForwardVel, 0.0f, 0.0f, 0.0f );
	glsLoadVector3( m_RightVel, 0.0f, 0.0f, 0.0f );
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Handles the collision reaction.
Parameters:
[in] _other - the entity to react against.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CPlayer::HandleReaction( void )
{
	std::vector<IEntity*> Col = this->GetCollisions();
	for(std::vector<IEntity*>::iterator iter = Col.begin(); iter != Col.end(); ++iter)
	{
		// Moving entity handles adjustment of position.
		CMovingEntity::HandleReaction();

		// This handles things that damage the player.
		//		NOT the adjustment of the players position based on collision
		//		that is what the previous call handles.
		switch((*iter)->GetType())
		{
		case ET_CRATE:
			{
			
			}
			break;
		case ET_SMALLENEMY:
			{
				if( ((CSmallEnemy*)(*iter))->GetState() != Enemy_Vulnerable && 
					((CSmallEnemy*)(*iter))->GetState() != Enemy_Stunned &&
					((CSmallEnemy*)(*iter))->GetState() != Enemy_Pushed &&
					((CSmallEnemy*)(*iter))->GetState() != Enemy_Dying )
				{
					TakeDamage( 25 );
					Vec3f damagePos;
					glsCopyVector3(damagePos, ((CBaseEntity*)(*iter))->GetRenderNode().GetPosition());
					CMessageSystem::GetInstance()->SendMessage(new CCreateVFXMessage(EMITTER_PLAYER_DAMAGE,this,damagePos[0],damagePos[1],damagePos[2]));
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_PLAYER_HURT);

				}
			}
			break;
		//case ET_MEDIUMENEMY:
		//	{
		//		if( ((CSmallEnemy*)(*iter))->GetState() != Enemy_Vulnerable && 
		//			((CSmallEnemy*)(*iter))->GetState() != Enemy_Stunned &&
		//			((CSmallEnemy*)(*iter))->GetState() != Enemy_Pushed &&
		//			((CSmallEnemy*)(*iter))->GetState() != Enemy_Dying )
		//		{
		//			//TakeDamage( 25 );
		//			Vec3f damagePos;
		//			glsCopyVector3(damagePos, ((CBaseEntity*)(*iter))->GetRenderNode().GetPosition());
		//			CMessageSystem::GetInstance()->SendMessage(new CCreateVFXMessage(EMITTER_PLAYER_DAMAGE,this,damagePos[0],damagePos[1],damagePos[2]));
		//		}
		//	}
		//	break;
		case ET_LIGHT_POLE:
		case ET_DEPOT:
		case ET_REDBARREL:
		case ET_SHIELD:
			{
				Sphere* other = ((Sphere*)(((CBaseEntity*)(*iter))->GetCollisionEntity()));
				Sphere* player = ((Sphere*)this->GetCollisionEntity());
							//Vec3f velocity;
				Vec3f OCPC;
				//glsCopyVector3(velocity, *this->GetVelocity());
				//Finds the vector between player's sphere and the static sphere
				glsSubtractVectors3(OCPC, player->m_Center, other->m_Center);
				glsNormalizeVector3(OCPC);
				glsScaleVector3(OCPC, other->m_Radius + player->m_Radius);
			
				Vec3f pos;
				glsAddVectors3(pos, ((Sphere*)((CBaseEntity*)(*iter))->GetCollisionEntity())->m_Center, OCPC);
				GetRenderNode().SetPosition( pos );

				//Vec3f planet = { 0.0f, 75.0f, 0.0f };
				//Adjust(planet);

				glsCopyVector3(*this->GetPosition(), this->GetRenderNode().GetPosition());
			}
			break;

		case ET_HEALTH_PICKUP_LARGE:
			{
				GainHealth( LARGE_HP_VALUE );
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_HEALTH_PICKUP);
				Vec3f temp;
				glsCopyVector3(temp, ((CBaseEntity*)(*iter))->GetRenderNode().GetPosition());
				CMessageSystem::GetInstance()->SendMessage(new CCreateVFXMessage(EMITTER_EXPLOSION,this,temp[0],temp[1],temp[2]));
				m_HealthPickup = 5.0f;
			}
			break;
		case ET_HEALTH_PICKUP_SMALL:
			{
				GainHealth( SMALL_HP_VALUE );
			}
			break;
		case ET_WORLD:
			{
				if( glsGetVectorLength3( this->m_UpVel ) > 100.0f )
				{
					glsLoadVector3( this->m_UpVel, 0.0f, 0.0f, 0.0f );
				}

				if( m_JumpTimer == 0.0f )
				{
					m_CanJump = true;
				}
			}
			break;
		case ET_EXPLOSION:
			{
				TakeDamage( EXPLOSION_DAMAGE );

				//if(this->GetHealth() > 0.0f)
				{
					// KNOCK BACK.
					Vec3f toExplode;
					glsSubtractVectors3(toExplode,this->GetRenderNode().GetPosition(),((CExplosion*)(*iter))->GetRenderNode().GetPosition());
					glsNormalizeVector3(toExplode);
					glsScaleVector3(toExplode,40.0f);
					
					glsAddVectors3(m_RightVel,m_RightVel,toExplode);
					glsAddVectors3(m_ForwardVel,m_ForwardVel,toExplode);
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_PLAYER_HURT);

				}
			}
			break;
		default:
			// send out error message to the system, stating problem in player HandleReaction
			bool error = true;
		};
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: The function that HURTS the player! His health goes down and then it checks
		 if he's still alive. If so, starts the death process; if not, does got damaged
		 stuff.
Parameters:
[in] _damage - the amount of damage to receive.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CPlayer::TakeDamage( int _damage )
{
	// if he's currently not invulnerable.
	if( GetInvulnerableTime() <= 0.0f )
	{
		if( this->GetHealth() > 0 )
		{
			this->SetHealth( this->GetHealth() - _damage );
			if( this->GetHealth() < 0 )
				this->SetHealth( 0 );
//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_2D_UNDERATTACK);
			//HAX
			//AUTH: Ryan
			//Desc: feedback for player hurt, replace with animation / effect later
			//Date: ????
			m_Hurt = true;
			GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_HURT), 0.2f);
			//this->GetRenderNode().hTextureDiffuse = CAssetManager::GetInstance()->GetSmallEnemyData(RED).m_DiffuseHandle;
		}
		else if( this->GetHealth() <= 0 )
			GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_DEATH), 0.1f);
		
		std::cout << "Player Health: " << this->GetHealth() << "/" << MAX_HP_PLAYER << std::endl;

		// TODO:: Animations
		if( this->GetHealth() != 0 )
		{
			// TODO:: Play damage sound
			// TODO:: Play damage animation
			// TODO:: Use damage effect
		}
		else
		{
			// TODO:: Play death sound
			// TODO:: Play Death animation
			// TODO:: Use Death effect
			// TODO:: Activate GAME OVER process.
			if( GetLives() > 1 )
			{
				m_DeathTimer = (float)(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_DEATH)->GetDuration() - 0.6f);
				//CMessageSystem::GetInstance()->SendMessage(new CCreateVFXMessage(GEO_EMITTER_BLOOD_POOL,this,0.0f,0.0f,0.0f));
			}
			else
				m_DeathTimer = (float)(CAssetManager::GetInstance()->GetAnimation(ET_PLAYER, PL_LOSE)->GetDuration() - 0.6f);

			std::cout << "PLAYER DYING!" << std::endl;
//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_3D_DEATH);
		}

		this->SetInvulnerableTime(1.0f);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: The function that HEALS the player! Pretty awesome right? This will take in an
		 amount of health to give the player, then add it to his health.
Parameters:
[in] _health - the amount of health to receive!
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CPlayer::GainHealth( int _health )
{
	if( (unsigned int)this->GetHealth() < MAX_HP_PLAYER )
	{
		this->SetHealth( (unsigned int)this->GetHealth() + _health );
		if( (unsigned int)this->GetHealth() > MAX_HP_PLAYER )
			this->SetHealth( MAX_HP_PLAYER );
	
		// TODO:: Play health gained sound.
		
		// TODO:: Use health pickup effect.
		std::cout << "Player Health: " << this->GetHealth() << "/" << MAX_HP_PLAYER << std::endl;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Moves the player right, using it's speed and it's current Right vector.
Parameters:
[in] _units - the amount to move.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CPlayer::MoveRight( float _units )
{
	m_RightVel[0] = m_MoveRight[0] * 20.0f * _units;
	m_RightVel[1] = m_MoveRight[1] * 20.0f * _units;
	m_RightVel[2] = m_MoveRight[2] * 20.0f * _units;
}

void CPlayer::RotateRight( float _units )
{
	Vec3f newForward, newRight;
	Matrix3x3f rotMatrix;
	glsRotationMatrix3x3( rotMatrix, _units*0.25f, this->GetRenderNode().GetUpVector()[0], this->GetRenderNode().GetUpVector()[1], this->GetRenderNode().GetUpVector()[2] );
	glsRotateVector( newForward, this->GetRenderNode().GetForwardVector(), rotMatrix );
	GetRenderNode().SetForwardVector( newForward );

	glsCrossProduct3( newRight, this->GetRenderNode().GetUpVector(), newForward );
	glsNormalizeVector3( newRight );
	GetRenderNode().SetRightVector( newRight );
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Moves the player forward, using it's speed and it's current Forward vector.
Parameters:
[in] _units - the amount to move.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CPlayer::MoveForward( float _units )
{
	m_ForwardVel[0] = m_MoveForward[0] * 20.0f * _units;
	m_ForwardVel[1] = m_MoveForward[1] * 20.0f * _units;
	m_ForwardVel[2] = m_MoveForward[2] * 20.0f * _units;
}

void CPlayer::RotateToPosition( Vec3f target, Matrix3x3f _camera )
{
	Vec3f testUp = { 0.0f, 1.0f, 0.0f};
	Vec3f right, fwd = { 0.0f, 1.0f, 0.0f };
	glsCrossProduct3( right, fwd, GetRenderNode().GetUpVector() );
	float angle = glsGetAngleBetweenVectors3( testUp, target );
	if( target[0] > 0.0f )
		angle *= -1.0f;
	Matrix3x3f testMatrix;
	glsRotationMatrix3x3( testMatrix, angle, GetRenderNode().GetUpVector()[0], GetRenderNode().GetUpVector()[1], GetRenderNode().GetUpVector()[2] );
	glsRotateVector( fwd, &_camera[3], testMatrix );
	glsNormalizeVector3( fwd );
	GetRenderNode().SetForwardVector( fwd );
	glsCrossProduct3( right, fwd, GetRenderNode().GetUpVector() );
	glsNormalizeVector3( right );
	GetRenderNode().SetRightVector( right );
}
