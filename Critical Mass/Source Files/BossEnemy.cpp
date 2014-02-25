/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : Boss.cpp
Author : Evan Wright

Description : The final challenge in the game! THE BOSS!

Created :  09/17/2013
Modified : 09/17/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "../Header Files/stdafx.h"
#include "../Header Files/BossEnemy.h"
#include "../Header Files/Shield.h"

#include "../Header Files/AssetManager.h"
#include "../Header Files/MessageSystem.h"
#include "../Header Files/CollisionEntity.h"
#include "../Header Files/CollisionLib.h"



#include "../Header Files/Explosion.h"

CBossEnemy::CBossEnemy( void )
{
	this->SetType( ET_BOSSENEMY );
	this->SetMaxHealth( MAX_HP_BOSS_ENEMY );

	// Default values for the render node's orientation
	GetRenderNode().SetUpVector( 0.0f, 1.0f, 0.0f );
	GetRenderNode().SetRightVector( 1.0f, 0.0f, 0.0f );
	GetRenderNode().SetForwardVector( 0.0f, 0.0f, 1.0f );

	// For movement...
	glsCopyVector3( m_MoveUp, this->GetRenderNode().GetUpVector() );
	glsCopyVector3( m_MoveRight, this->GetRenderNode().GetRightVector() );
	glsCopyVector3( m_MoveForward, this->GetRenderNode().GetForwardVector() );

	// COLLISION DATA
	Vec3f pos = { 0.0f, 0.0f, 0.0f };
	this->SetCollisionEntity( new Sphere( 4.0f, pos ) );
	this->SetExperienceValue(100000); // lolololol why not?

	Reset();
}

void CBossEnemy::Init( void )
{
	GetRenderNode().SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[LARGE_ENEMY_SHAPE_TB] );
	GetRenderNode().SetDiffuseTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_BOSSENEMY).m_DiffuseHandle );
	GetRenderNode().SetSpecularTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_LARGEENEMY).m_hTextureSpecular );

	GetRenderNode().SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[ANIMATED_ONLY] );
	GetRenderNode().SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[ANIMATED_ONLY] );
	GetRenderNode().SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[ANIMATED_ONLY] );
	GetRenderNode().SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[ANIMATED_ONLY] );
	GetRenderNode().SetSkinningMatrixUniformHandle( CAssetManager::GetInstance()->m_vUniformJointHandles[ANIMATED_ONLY] );

	GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_BOSSENEMY, LARGE_IDLE), 0.1f);

	GetRenderNode().SetDissolveFactor( 1.0f );
}

void CBossEnemy::Reset( void )
{
	this->m_LaserTimer = 0.0f;
	this->m_DeathTimer = 0.0f;
	this->m_HurtTimer = 0.0f;
	this->m_JustSpawnedTimer = 1.0f;
	this->m_Phase = 0;
	this->m_ShieldFadeIn = 0.0f;
	this->m_ShieldFadeWait = 0.0f;
	this->m_Shield = nullptr;
	this->m_NoShield = true;
	this->SetMoveSpeed( 20 );
	this->SetState( Enemy_Chasing );
	this->SetHealth( MAX_HP_BOSS_ENEMY );

	m_TauntIDs.clear();
	m_TauntIDs.push_back(AK::EVENTS::PLAY_DX_BOSS_EXTERMINATE);
	m_TauntIDs.push_back(AK::EVENTS::PLAY_DX_BOSS_KILL);
	m_TauntIDs.push_back(AK::EVENTS::PLAY_DX_BOSS_PLANETS);
	m_TauntIDs.push_back(AK::EVENTS::PLAY_DX_BOSS_RESISTANCE);

	GetRenderNode().SetEnableBlending( false );
}

void CBossEnemy::UpdateVelocity( void )
{
	Vec3f totalUp, totalForward, totalRight, totalVel;
	glsCopyVector3( totalUp, m_MoveUp );
	glsCopyVector3( totalRight, m_MoveRight );
	glsCopyVector3( totalForward, m_MoveForward );
	glsLoadVector3( totalVel, 0.0f, 0.0f, 0.0f );

	glsScaleVector3( totalUp, 1.0f );
	glsScaleVector3( totalRight, *this->GetVelocity()[0] );
	glsScaleVector3( totalForward, *this->GetVelocity()[2] );

	glsAddVectors3( totalVel, totalVel, totalUp );
	glsAddVectors3( totalVel, totalVel, totalRight );
	glsAddVectors3( totalVel, totalVel, totalForward );
}

void CBossEnemy::TakeDamage( int _amount )
{
	// TODO:: PHASE CHECK HERE, CHECK FOR SHIELD
	if( this->GetHealth() > 0 )
	{
		this->SetHealth( this->GetHealth() - _amount );
		AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_BOSS_DAMAGE);
	}
	m_HurtTimer = BOSS_ENEMY_TIMER_HURT;
}

void CBossEnemy::Kill( void )
{
	this->SetIsActive( false );
	this->SetIsAlive( false );
	this->SetIsDying( true );

	CMessageSystem::GetInstance()->SendMessage( new CCreateExplosionMessage( this, ET_BOSSENEMY, true, 4.0f ) );

	if(this->m_Shield != nullptr)
	{
		((CShield*)m_Shield)->ResetShield();
		m_Shield->SetIsActive(false);
		m_Shield->SetIsAlive(false);
		m_Shield = nullptr;
	}

	Reset();
}

void CBossEnemy::PlayRandomTaunt( void )
{
	int index = rand()%m_TauntIDs.size();
	AkUniqueID sound_id = m_TauntIDs[index];
	m_TauntIDs.erase(m_TauntIDs.begin()+index);
	
	AudioSystemWwise::GetInstance()->PostEvent(sound_id);
}

void CBossEnemy::Release( void )
{
	
}

bool CBossEnemy::Update( float _elapsedTime )
{
	// TODO:: PHASES
	//if(this->m_Shield)
	//{
	//	if(this->m_Shield->GetIsActive() && this->m_Shield->GetIsAlive())
	//	{
	//		m_Shield->GetRenderNode().SetPosition( this->GetRenderNode().GetPosition() );
	//		m_Shield->GetRenderNode().SetForwardVector(0.0f,0.0f,-1.0f);
	//		m_Shield->GetRenderNode().SetRightVector(1.0f,0.0f,0.0f);
	//		m_Shield->GetRenderNode().SetUpVector(0.0f,1.0f,0.0f);
	//
	//		Vec4f fadeColor; 
	//		glsCopyVector4(fadeColor,m_Shield->GetRenderNode().GetColor());
	//		fadeColor[3] = 0.0f;
	//		if(m_ShieldFadeWait <= 0.0f && m_ShieldFadeIn > 0.0f)
	//		{
	//			if(m_ShieldFadeIn == BOSS_SHIELD_FADE_IN)
	//			{
	//				if(m_Phase == 1)
	//					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_BOSS_STATE_01);
	//				else if(m_Phase == 2)
	//					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_BOSS_STATE_02);
	//			}
	//
	//			fadeColor[3] = (BOSS_SHIELD_FADE_IN-m_ShieldFadeIn)/BOSS_SHIELD_FADE_IN * 0.5f;
	//			m_ShieldFadeIn -= _elapsedTime;
	//		}
	//		else if(m_ShieldFadeWait > 0.0f)
	//			m_ShieldFadeWait -= _elapsedTime;
	//		else if(m_ShieldFadeIn <= 0.0f)
	//			fadeColor[3] = 0.5f;
	//
	//		m_Shield->GetRenderNode().SetColor(fadeColor);
	//
	//		((Sphere*)m_Shield->GetCollisionEntity())->m_Radius = 25.0f;
	//		m_NoShield = false;
	//	}
	//	else
	//		m_NoShield = true;
	//}
	
	//if(m_NoShield && m_Phase == 3 && m_ShieldFadeIn != 0.0f && this->GetHealth() > 0.0f)
	//{
	//	m_ShieldFadeWait -= _elapsedTime;
	//	if(m_ShieldFadeWait < 0.0f)
	//	{
	//		AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_BOSS_STATE_03);
	//		m_ShieldFadeIn = 0.0f;
	//		m_ShieldFadeWait = 0.0f;
	//	}
	//}
	
	if((m_Phase < 3 && this->GetHealth() <= 0) || m_Phase == 0)
	{
		m_Phase++;
		m_PhaseTauntPlayed = false;
		CMessageSystem::GetInstance()->SendMessageW( new CBossPhaseChangeMessage(m_Phase,this));
		//m_ShieldFadeIn = BOSS_SHIELD_FADE_IN;
		//m_ShieldFadeWait = BOSS_SHIELD_FADE_WAIT;
		this->SetHealth(MAX_HP_BOSS_ENEMY);
		if(m_Phase > 1)
			CMessageSystem::GetInstance()->SendMessage(new CCreateVFXMessage(GEO_EMITTER_ENERGY_RING,this,0.0f,0.0f,0.0f));
	}
	
	// For playing taunts
	if(!m_PhaseTauntPlayed)
	{
		{
			if(this->GetHealth() < MAX_HP_BOSS_ENEMY/2.0f)
			{
				PlayRandomTaunt();
				m_PhaseTauntPlayed = true;
			}
		}
	}

	// If they are preparing an attack.
	if( this->GetState() == Enemy_Charging )
	{
		static float tell_blink = 0.2f;
		tell_blink += _elapsedTime;
		if( tell_blink >= 0.2f )
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
		this->GetRenderNode().SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		if(m_DeathTimer > 0.0f)
		{
			GetAnimationManager()->SwitchAnimation(CAssetManager::GetInstance()->GetAnimation(ET_BOSSENEMY, LARGE_DEATH), 0.1f);
			if(m_DeathTimer == BOSS_ENEMY_TIMER_DEATH)
			{
				CMessageSystem::GetInstance()->SendMessageW(new CBossOutroMessage(this));
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_BOSS_DEATH);
			}

			m_ExplosionInterval += _elapsedTime;
			if(m_ExplosionInterval >= BOSS_ENEMY_TIMER_E_INTERVAL)
			{
				float randSize = float(rand()%100)/75.0f + 1.0f;
				CMessageSystem::GetInstance()->SendMessage(new CCreateExplosionMessage(this,ET_BOSSENEMY,true,1.0f));
				m_ExplosionInterval = 0.0f;
			}

			m_DeathTimer -= _elapsedTime;
		}
		else
		{
			Kill();
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
	glsCopyVector3( m_MoveUp, this->GetRenderNode().GetUpVector() );
	glsCrossProduct3( m_MoveForward, m_MoveRight, m_MoveUp );
	glsNormalizeVector3( m_MoveForward );
	glsCrossProduct3( m_MoveRight, m_MoveUp, m_MoveForward );
	glsNormalizeVector3( m_MoveRight );

	UpdateVelocity();
	CEnemy::Update( _elapsedTime );

	// Death stuff:
	if( this->GetHealth() <= 0 && this->GetState() != Enemy_Dying && m_Phase == 3 )
	{
		this->SetState( Enemy_Dying );
		m_DeathTimer = BOSS_ENEMY_TIMER_DEATH;
	}

	if( m_JustSpawnedTimer > 0.0f )
	{
		m_JustSpawnedTimer -= _elapsedTime;
	}

	return true;
}

void CBossEnemy::HandleReaction( void )
{
	bool LargeCol = false;
	Vec3f sumMove = { 0.0f, 0.0f, 0.0f };
	std::vector<IEntity*> Col = this->GetCollisions();
	for( std::vector<IEntity*>::iterator iter = Col.begin(); iter != Col.end(); ++iter )
	{
		switch( (*iter)->GetType() )
		{
		case ET_CRATE:
		case ET_REDBARREL:
		case ET_SMALLENEMY:
		case ET_MEDIUMENEMY:
		case ET_LARGEENEMY:
		case ET_SHIELD:
			{
				if( this->GetState() == Enemy_Charging || m_JustSpawnedTimer > 0.0f || (*iter) == m_Shield )
					continue;

				Sphere* other = ((Sphere*)(((CBaseEntity*)(*iter))->GetCollisionEntity()));
				Sphere* smEnemy = ((Sphere*)this->GetCollisionEntity());
				//Vec3f velocity;
				Vec3f OCPC;
				//Finds the vector between player's sphere and the static sphere
				glsSubtractVectors3( OCPC, this->GetRenderNode().GetPosition(), ((CBaseEntity*)(*iter))->GetRenderNode().GetPosition() );
				glsNormalizeVector3( OCPC );
				float dist = (other->m_Radius + smEnemy->m_Radius) - glsGetDistance3( this->GetRenderNode().GetPosition(), ((CBaseEntity*)(*iter))->GetRenderNode().GetPosition() );
				glsScaleVector3(OCPC, dist);
				// if the sumMove hasn't be set then just use the first vector
				if( !LargeCol )
				{
					glsCopyVector3( sumMove, OCPC );
					LargeCol = true;
				}
				else
				{
					// Otherwise find the largest part for each axis of the vector.
					for( int i = 0; i < 3; ++i )
					{
						if( abs( OCPC[i] ) > abs( sumMove[i] ) )
							sumMove[i] = OCPC[i];
					}
				}
			}
			break;
		case ET_WORLD:
			{
				if( this->GetState() == Enemy_Dying )
					continue;

				Sphere* other = ((Sphere*)(((CBaseEntity*)(*iter))->GetCollisionEntity()));
				Sphere* smEnemy = ((Sphere*)this->GetCollisionEntity());
				//Vec3f velocity;
				Vec3f OCPC;
				//Finds the vector between player's sphere and the static sphere
				glsSubtractVectors3( OCPC, this->GetRenderNode().GetPosition(), ((CBaseEntity*)(*iter))->GetRenderNode().GetPosition() );
				glsNormalizeVector3( OCPC );
				float dist = (other->m_Radius + smEnemy->m_Radius) - glsGetDistance3( this->GetRenderNode().GetPosition(), ((CBaseEntity*)(*iter))->GetRenderNode().GetPosition() );
				glsScaleVector3( OCPC, dist );
				// if the sumMove hasn't be set then just use the first vector
				if( !LargeCol )
				{
					glsCopyVector3( sumMove, OCPC );
					LargeCol = true;
				}
				else
				{
					// Otherwise find the largest part for each axis of the vector.
					for(int i = 0; i < 3; i++)
					{
						if( abs( OCPC[i] ) > abs( sumMove[i] ) )
							sumMove[i] = OCPC[i];
					}
				}
			}
			break;
		case ET_BULLET_PLAYER:
			{
				if(m_Shield && m_Phase < 3)
					continue;

				TakeDamage( YELLOW_DAMAGE );
			}
			break;
		case ET_BLUESTRIKE:
			{
				if(m_Shield && m_Phase < 3)
					continue;

				TakeDamage( 100 );
			}
			break;
		case ET_EXPLOSION:
			{
				if(m_Shield && m_Phase < 3)
					continue;

				if(!((CExplosion*)(*iter))->GetIsEMP())
				{
					TakeDamage(EXPLOSION_DAMAGE);
					if(this->GetHealth() <= 0 && ((CExplosion*)(*iter))->GetOwner() != ET_REDBARREL)
						CMessageSystem::GetInstance()->SendMessageW(new CGiveExperienceMessage(EXP_LARGE_ENEMY,RED));

					//if(this->GetHealth() > 0.0f)
					{
						// KNOCK BACK.
						Vec3f toExplode;
						glsSubtractVectors3(toExplode,this->GetRenderNode().GetPosition(),((CExplosion*)(*iter))->GetRenderNode().GetPosition());
						glsNormalizeVector3(toExplode);
						glsScaleVector3(toExplode,40.0f);
						
						glsAddVectors3(*(this->GetVelocity()),*(this->GetVelocity()),toExplode);
					}
				}
			}
			break;
		case ET_LASER:
			{
				if(m_Shield && m_Phase < 3)
					continue;
			
				if( m_LaserTimer == 0.0f )
				{
					TakeDamage( BLUE_DAMAGE );
					m_LaserTimer = 0.1f;

					if( GetHealth() <= 0.0f )
					{
						GetRenderNode().SetEnableBlending( true );
						GetRenderNode().SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[DISSOLVE] );
						GetRenderNode().SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[DISSOLVE] );
						GetRenderNode().SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[DISSOLVE] );
						GetRenderNode().SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[DISSOLVE] );
						GetRenderNode().SetTextureUnitHandle1( CAssetManager::GetInstance()->m_vTextureUnit1Handles[DISSOLVE] );
						GetRenderNode().SetDissolveFactorUniformHandle( CAssetManager::GetInstance()->m_vUniformDissolveFactorHandles[DISSOLVE] );
						GetRenderNode().SetSpecularTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_SHIELD).m_hTextureSpecular );
						SetLaserDeath( true );
					}
				}
				break;
			}
			break;
		}
	}

	if( LargeCol )
	{
		Vec3f pos;
		glsAddVectors3( pos, this->GetRenderNode().GetPosition(), sumMove );
		glsCopyVector3( *this->GetPosition(), pos );
		GetRenderNode().SetPosition( pos );
	}
}

