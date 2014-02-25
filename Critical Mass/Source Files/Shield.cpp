#include "../Header Files/StdAfx.h"
#include "../Header Files/Shield.h"
#include "../Header Files/Explosion.h"
#include "../Header Files/CollisionEntity.h"
#include "../Header Files/Definitions.h"
#include "../Header Files/GLSetup.h"
#include "../Header Files/CollisionLib.h"
#include "../Header Files/glsMath.h"
//SoundManager includes....................................
#include "../../Includes/Sound/Source/Wwise_IDs.h"
#include "../../Includes/Sound/Source/AudioSystemWwise.h"
//.........................................................
#include "../Header Files/AssetManager.h"
#include "../Header Files/MessageSystem.h"
#include "../Header Files/Game.h"

CShield::CShield(void)
{
	CBaseEntity::SetType(ET_SHIELD);

	//Collision Check
	Vec3f pos = { 0.0f, 0.0f, 0.0f };

	this->SetCollisionEntity(new Sphere(0.0f,pos));
	Sphere there = *((Sphere*)this->GetCollisionEntity());

	// Default Orientation.
	GetRenderNode().SetUpVector( 0.0f, 1.0f, 0.0f );
	GetRenderNode().SetRightVector( 1.0f, 0.0f, 0.0f );
	GetRenderNode().SetForwardVector( 0.0f, 0.0f, 1.0f );

	m_InvulnerableTimer = 0.0f;
	m_LaserTimer = 0.0f;
	m_ineffectiveHitTimer = 0.0f;
	m_ineffective = false;
	m_effective = false;
	//m_flicker = false;
}

void CShield::Initialize( double _X, double _Y, bool _invulnerable )
{
	ResetShield();
	this->m_awarded[0] = false;
	this->m_awarded[1] = false;
	this->m_awarded[2] = false;
	this->SetIsActive(true);
	this->SetIsAlive(true);
	//this->SetPosition( _position );
	//this->SetWorldPosition (_worldPosition);
	m_Health = MAX_HP_SHIELD;	//MAX_HP_SHIELD = 100
	m_invulnerable = _invulnerable;

	//Calculation for the rotation of the Shield
	Vec3f up, fwd, right;
	Matrix3x3f rotMatrixX, rotMatrixY;

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

	Vec3f final;
	glsScaleVector3( up, -75 );
	glsAddVectors3( final, up, _moon );
	GetRenderNode().SetPosition( final );

	glsCopyVector3(((Sphere*)(this->GetCollisionEntity()))->m_Center, final);
	((Sphere*)GetCollisionEntity())->m_Radius = 10.0f;

	Vec3f id;
	glsLoadVector3( id, 0.0f, 0.0f, 0.0f );

	////ADD SHIELD to depot
	//glsCreateSphere(*(GetRenderNode().triangleBatch), 25.0f);
	GetRenderNode().SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[SHIELD_TB] );

	//this->GetRenderNode().hTextureDiffuse = 3;

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

	this->GetRenderNode().SetColor(0.0f,1.0f,0.2f,0.4f);

	m_InvulnerableTimer = 0.0f;
	m_LaserTimer = 0.0f;
	m_ineffectiveHitTimer = 0.0f;
	m_effectiveHitTimer = 0.0f;
}

float CShield::GetCollisionSize(void)
{
	return ((Sphere*)GetCollisionEntity())->m_Radius;
}

void CShield::ResizeCollisionEntity(float _size)
{
	((Sphere*)GetCollisionEntity())->m_Radius =_size;
}

void CShield::InitShaders()
{
	GetRenderNode().SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[SHIELD_TB] );
	GetRenderNode().SetDiffuseTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_SHIELD).m_DiffuseHandle );
	GetRenderNode().SetSpecularTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_SHIELD).m_hTextureSpecular );
	GetRenderNode().SetEnableBlending( true );

	GetRenderNode().SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[DISSOLVE] );
	GetRenderNode().SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[DISSOLVE] );
	GetRenderNode().SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[DISSOLVE] );
	GetRenderNode().SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[DISSOLVE] );
	GetRenderNode().SetTextureUnitHandle1( CAssetManager::GetInstance()->m_vTextureUnit1Handles[DISSOLVE] );
	GetRenderNode().SetDissolveFactorUniformHandle( CAssetManager::GetInstance()->m_vUniformDissolveFactorHandles[DISSOLVE] );
}

bool CShield::Update (float _elapsedTime )
{
	CBaseEntity::Update(_elapsedTime);

	// There's a bug here.  Not all control paths in this series of if-statements
	// actually sets the red, green, and ratio values.  Therefore, I've put a
	// band-aid on it by initializing the values of the appropriate variables.
	// We need to come back here and fix the underlying problem at some point. -B(ob Pasekoff)
	if(!m_invulnerable)
	{
		float temp,tempRatio = 1.0f,tempRed = 0.0f,tempgreen = 1.0f;
		if(CGame::GetInstance()->GetDifficulty() != HARD_DIFF)
		{
			if(this->m_Health < (int)(MAX_HP_SHIELD*2))
			{
				temp = MAX_HP_SHIELD * 2.0f;
				tempRatio = (float)(m_Health / temp);
				tempRed =  this->GetRenderNode().GetColor()[0];
				tempgreen = this->GetRenderNode().GetColor()[1];
				tempRed = 1.0f - tempRatio;
				tempgreen = tempRatio;
				if(tempgreen < 0.0f)
					tempgreen = 0.0f;
				if(tempRed > 1.0f)
					tempRed = 1.0f;
				this->GetRenderNode().SetColor(tempRed,tempgreen,0.2f,0.4f);
			}
		}
		else
		{
			if(this->m_Health < (int)(MAX_HP_SHIELD*3))
			{
				temp = MAX_HP_SHIELD * 3.0f;
				tempRatio = (float)(m_Health / temp);
				tempRed =  this->GetRenderNode().GetColor()[0];
				tempgreen = this->GetRenderNode().GetColor()[1];
				tempRed = 1.0f - tempRatio;
				tempgreen = tempRatio;
				if(tempgreen < 0.0f)
					tempgreen = 0.0f;
				if(tempRed > 1.0f)
					tempRed = 1.0f;
				this->GetRenderNode().SetColor(tempRed,tempgreen,0.2f,0.4f);
			}
		}
		if((m_ineffectiveHitTimer >= 0.0f) && (m_ineffective == true))
		{
			m_ineffectiveHitTimer -= _elapsedTime;
			GetRenderNode().SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[RIPPLE_EFFECT] );
			GetRenderNode().SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[RIPPLE_EFFECT] );
			GetRenderNode().SetMVUniformHandle( CAssetManager::GetInstance()->m_vUniformMVHandles[RIPPLE_EFFECT] );
			GetRenderNode().SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformCameraHandles[RIPPLE_EFFECT] );
			GetRenderNode().SetTimeUniformHandle( CAssetManager::GetInstance()->m_vUniformTimeHandles[RIPPLE_EFFECT] );
			GetRenderNode().SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[RIPPLE_EFFECT] );
			GetRenderNode().SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[RIPPLE_EFFECT] );
			GetRenderNode().SetHitPositionUniformHandle( CAssetManager::GetInstance()->m_vUniformHitPositionHandles[RIPPLE_EFFECT] );
		}
		else if (m_ineffectiveHitTimer < 0.0f && (m_ineffective == true))
		{
			m_ineffectiveHitTimer = 0.0f;
			m_ineffective = false;

			GetRenderNode().SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[DISSOLVE] );
			GetRenderNode().SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[DISSOLVE] );
			GetRenderNode().SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[DISSOLVE] );
			GetRenderNode().SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[DISSOLVE] );
			GetRenderNode().SetTextureUnitHandle1( CAssetManager::GetInstance()->m_vTextureUnit1Handles[DISSOLVE] );
			GetRenderNode().SetDissolveFactorUniformHandle( CAssetManager::GetInstance()->m_vUniformDissolveFactorHandles[DISSOLVE] );
		}

		if(m_effectiveHitTimer > 0.0f && (m_effective == true))
		{
			m_effectiveHitTimer -= _elapsedTime;

			GetRenderNode().SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[RIPPLE_EFFECT] );
			GetRenderNode().SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[RIPPLE_EFFECT] );
			GetRenderNode().SetMVUniformHandle( CAssetManager::GetInstance()->m_vUniformMVHandles[RIPPLE_EFFECT] );
			GetRenderNode().SetCameraUniformHandle( CAssetManager::GetInstance()->m_vUniformCameraHandles[RIPPLE_EFFECT] );
			GetRenderNode().SetTimeUniformHandle( CAssetManager::GetInstance()->m_vUniformTimeHandles[RIPPLE_EFFECT] );
			GetRenderNode().SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[RIPPLE_EFFECT] );
			GetRenderNode().SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[RIPPLE_EFFECT] );
			GetRenderNode().SetHitPositionUniformHandle( CAssetManager::GetInstance()->m_vUniformHitPositionHandles[RIPPLE_EFFECT] );
			this->GetRenderNode().SetColor(tempRed,tempgreen,0.2f,0.3f);
		}
		else if (m_effectiveHitTimer <= 0.0f && (m_effective == true))
		{
			m_effective = false;
			m_effectiveHitTimer = 0.0f;

			//GetRenderNode().hShader = CAssetManager::GetInstance()->m_vShaderHandles[TEXTURE_ONLY];
			//GetRenderNode().hUniformMVP = CAssetManager::GetInstance()->m_vUniformMVPHandles[TEXTURE_ONLY];
			//GetRenderNode().hUniformColor = CAssetManager::GetInstance()->m_vUniformColorHandles[TEXTURE_ONLY];
			//GetRenderNode().hTextureUnit0 = CAssetManager::GetInstance()->m_vTextureUnit0Handles[TEXTURE_ONLY];

			GetRenderNode().SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[DISSOLVE] );
			GetRenderNode().SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[DISSOLVE] );
			GetRenderNode().SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[DISSOLVE] );
			GetRenderNode().SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[DISSOLVE] );
			GetRenderNode().SetTextureUnitHandle1( CAssetManager::GetInstance()->m_vTextureUnit1Handles[DISSOLVE] );
			GetRenderNode().SetDissolveFactorUniformHandle( CAssetManager::GetInstance()->m_vUniformDissolveFactorHandles[DISSOLVE] );
			this->GetRenderNode().SetColor(tempRed,tempgreen,0.2f,0.5f);
		}
		if( m_InvulnerableTimer > 0.0f )
		{
			m_InvulnerableTimer -= _elapsedTime;
			if( m_InvulnerableTimer < 0.0f )
				m_InvulnerableTimer = 0.0f;
		}
		if( m_LaserTimer > 0.0f )
		{
			m_LaserTimer -= _elapsedTime;
			if( m_LaserTimer < 0.0f )
				m_LaserTimer = 0.0f;
		}
	}
	else
	{
		this->GetRenderNode().SetColor(163.0f/255.0f,73.0f/255.0f,164.0f/255.0f,0.5f);
	}

	if( m_Health <= 0 )
	{
		GetRenderNode().SetDissolveFactor( GetRenderNode().GetDissolveFactor() - _elapsedTime / 2.5f );
		if(GetRenderNode().GetDissolveFactor() <= 0.0f)
		{
			this->SetIsActive(false);
			this->SetIsAlive(false);
			std::cout << "SHIELD IS DESTROYED!" << std::endl;
//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_3D_WEAPONWHOOSH);
		}

		GetRenderNode().SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[DISSOLVE] );
		GetRenderNode().SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[DISSOLVE] );
		GetRenderNode().SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[DISSOLVE] );
		GetRenderNode().SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[DISSOLVE] );
		GetRenderNode().SetTextureUnitHandle1( CAssetManager::GetInstance()->m_vTextureUnit1Handles[DISSOLVE] );
		GetRenderNode().SetDissolveFactorUniformHandle( CAssetManager::GetInstance()->m_vUniformDissolveFactorHandles[DISSOLVE] );
	}

	glsCopyVector3(*GetPosition(), GetRenderNode().GetPosition());
	glsCopyVector3(((Sphere*)this->GetCollisionEntity())->m_Center, this->GetRenderNode().GetPosition() );
	return true;
}

void CShield::HandleReaction ( void )
{
	std::vector<IEntity*> Col = this->GetCollisions();
	for(std::vector<IEntity*>::iterator iter = Col.begin(); iter != Col.end(); ++iter)
	{
		GetRenderNode().SetHitPosition( ((CBaseEntity*)(*iter))->GetRenderNode().GetPosition() );

		switch((*iter)->GetType())
		{
		case ET_BULLET_PLAYER:
			{
				this->TakeDamage(YELLOW_DAMAGE);
				if(CGame::GetInstance()->GetDifficulty() != HARD_DIFF)
				{
					if(this->m_Ownership == ET_DEPOT)
					{
						if((unsigned int)this->GetHealth() < (MAX_HP_SHIELD * 2) &&  (unsigned int)this->GetHealth() > (unsigned int)((MAX_HP_SHIELD * 2) / 3) && !m_awarded[0])
						{
							CMessageSystem::GetInstance()->SendMessageW(new CGiveExperienceMessage(EXP_DEPOT / 3,YELLOW));
							m_awarded[0] = true;
							cout << "Exp from depot, yellow weapon , 66%" << endl;
							cout << "Health: " << this->GetHealth() << " out of " << MAX_HP_SHIELD * 2 << endl;
						}
						if((unsigned int)this->GetHealth() < (unsigned int)((MAX_HP_SHIELD * 2) / 3) &&  (unsigned int)this->GetHealth() > (unsigned int)((MAX_HP_SHIELD * 2) / 6) && !m_awarded[1])
						{
							CMessageSystem::GetInstance()->SendMessageW(new CGiveExperienceMessage(EXP_DEPOT / 3,YELLOW));
							m_awarded[1] = true;
							cout << "Exp from depot, yellow weapon , 33%" << endl;
							cout << "Health: " << this->GetHealth() << " out of " << MAX_HP_SHIELD * 2 << endl;
						}
					}
				}
				else
				{
					if(this->m_Ownership == ET_DEPOT)
					{
						if((unsigned int)this->GetHealth() < (MAX_HP_SHIELD * 3) &&  (unsigned int)this->GetHealth() > (unsigned int)((MAX_HP_SHIELD * 3) / 3) && !m_awarded[0])
						{
							CMessageSystem::GetInstance()->SendMessageW(new CGiveExperienceMessage(EXP_DEPOT / 3,YELLOW));
							m_awarded[0] = true;
						}
						if((unsigned int)this->GetHealth() < ((MAX_HP_SHIELD * 3) / 3)  &&  (unsigned int)this->GetHealth() > (unsigned int)((MAX_HP_SHIELD * 3) / 6) && !m_awarded[1])
						{
							CMessageSystem::GetInstance()->SendMessageW(new CGiveExperienceMessage(EXP_DEPOT / 3,YELLOW));
							m_awarded[1] = true;
						}
					}
				}
				if(this->GetHealth() <= 0.0f)
				{
					if(this->m_Ownership == ET_DEPOT && !m_awarded[2])
					{
						CMessageSystem::GetInstance()->SendMessageW(new CGiveExperienceMessage(EXP_DEPOT / 3,YELLOW));
						this->m_awarded[2] = true;
						cout << "Exp from depot, yellow weapon" << endl;
					}
				}
				m_effectiveHitTimer = 1.0f;
				m_effective = true;
//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MAGICPICKUP);
				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SHIELD_PROJECTILE_DAMAGE);
				break;
			}
		case ET_EXPLOSION:
			{
				if(m_InvulnerableTimer == 0.0f && !((CExplosion*)(*iter))->GetIsEMP())
				{
					this->TakeDamage(EXPLOSION_DAMAGE);
					if(CGame::GetInstance()->GetDifficulty() != HARD_DIFF)
					{
						if(this->m_Ownership == ET_DEPOT)
						{
							if((unsigned int)this->GetHealth() < (MAX_HP_SHIELD * 2) &&  (unsigned int)this->GetHealth() > (unsigned int)((MAX_HP_SHIELD * 2) / 3) && !m_awarded[0])
							{
								CMessageSystem::GetInstance()->SendMessageW(new CGiveExperienceMessage(EXP_DEPOT / 3,RED));
								m_awarded[0] = true;
								cout << "Exp from depot, red weapon , 66%" << endl;
							}
							if((unsigned int)this->GetHealth() < (unsigned int)((MAX_HP_SHIELD * 2) / 3) &&  (unsigned int)this->GetHealth() > (unsigned int)((MAX_HP_SHIELD * 2) / 6) && !m_awarded[1])
							{
								CMessageSystem::GetInstance()->SendMessageW(new CGiveExperienceMessage(EXP_DEPOT / 3,RED));
								m_awarded[1] = true;
								cout << "Exp from depot, red weapon , 33%" << endl;
							}
						}
					}
					else
					{
						if(this->m_Ownership == ET_DEPOT)
						{
							if((unsigned int)this->GetHealth() < (MAX_HP_SHIELD * 3) &&  (unsigned int)this->GetHealth() > (unsigned int)((MAX_HP_SHIELD * 3) / 3) && !m_awarded[0])
							{
								CMessageSystem::GetInstance()->SendMessageW(new CGiveExperienceMessage(EXP_DEPOT / 3,RED));
								m_awarded[0] = true;
							}
							if((unsigned int)this->GetHealth() < (unsigned int)((MAX_HP_SHIELD * 3) / 3)  &&  (unsigned int)this->GetHealth() > (unsigned int)((MAX_HP_SHIELD * 3) / 6) && !m_awarded[1])
							{
								CMessageSystem::GetInstance()->SendMessageW(new CGiveExperienceMessage(EXP_DEPOT / 3,RED));
								m_awarded[1] = true;
							}
						}
					}
					if(this->GetHealth() <= 0.0f)
					{
						if(this->m_Ownership == ET_DEPOT && !m_awarded[2])
						{
							CMessageSystem::GetInstance()->SendMessageW(new CGiveExperienceMessage(EXP_DEPOT / 3,RED));
							this->m_awarded[2] = true;
							cout << "Exp from depot, red weapon" << endl;
						}
					}
					m_effectiveHitTimer = 1.0f;
					m_effective = true;
////					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MAGICPICKUP);
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_SHIELD_EXPLOSIVE_DAMAGE);

					m_InvulnerableTimer = 0.5f;
				}
				break;
			}
		case ET_LASER:
			{
				if(m_LaserTimer == 0.0f)
				{
					this->TakeDamage(BLUE_DAMAGE);
					if(CGame::GetInstance()->GetDifficulty() != HARD_DIFF)
					{
						if(this->m_Ownership == ET_DEPOT)
						{
							if((unsigned int)this->GetHealth() < (MAX_HP_SHIELD * 2) &&  (unsigned int)this->GetHealth() > (unsigned int)((MAX_HP_SHIELD * 2) / 3) && !m_awarded[0])
							{
								CMessageSystem::GetInstance()->SendMessageW(new CGiveExperienceMessage(EXP_DEPOT / 3,BLUE));
								m_awarded[0] = true;
								cout << "Exp from depot, blue weapon end , 66%" << endl;
							}
							if((unsigned int)this->GetHealth() < (unsigned int)((MAX_HP_SHIELD * 2) / 3) &&  (unsigned int)this->GetHealth() > (unsigned int)((MAX_HP_SHIELD * 2) / 6) && !m_awarded[1])
							{
								CMessageSystem::GetInstance()->SendMessageW(new CGiveExperienceMessage(EXP_DEPOT / 3,BLUE));
								m_awarded[1] = true;
								cout << "Exp from depot, blue weapon end , 33%" << endl;
							}
						}
					}
					else
					{
						if(this->m_Ownership == ET_DEPOT)
						{
							if((unsigned int)this->GetHealth() < (MAX_HP_SHIELD * 3) &&  (unsigned int)this->GetHealth() > (unsigned int)((MAX_HP_SHIELD * 3) / 3) && !m_awarded[0])
							{
								CMessageSystem::GetInstance()->SendMessageW(new CGiveExperienceMessage(EXP_DEPOT / 3,BLUE));
								m_awarded[0] = true;
							}
							if((unsigned int)this->GetHealth() < (unsigned int)((MAX_HP_SHIELD * 3) / 3)  &&  (unsigned int)this->GetHealth() > (unsigned int)((MAX_HP_SHIELD * 3) / 6) && !m_awarded[1])
							{
								CMessageSystem::GetInstance()->SendMessageW(new CGiveExperienceMessage(EXP_DEPOT / 3,BLUE));
								m_awarded[1] = true;
							}
						}
					}
					if(this->GetHealth() <= 0.0f)
					{
						if(this->m_Ownership == ET_DEPOT && !m_awarded[2])
						{
							CMessageSystem::GetInstance()->SendMessageW(new CGiveExperienceMessage(EXP_DEPOT / 3,BLUE));
							this->m_awarded[2] = true;
							cout << "Exp from depot, blue weapon end" << endl;
						}
					}
					m_effective = true;
					m_effectiveHitTimer = 1.0f;
					m_LaserTimer = 0.1f;
//					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MAGICPICKUP);
					AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SHIELD_PROJECTILE_DAMAGE);
				}
				break;
			}
		default:
			{
				bool error = true;
			}
		}
	}
}


void CShield::TakeDamage ( int _damage )
{
	// This isn't even being used....
	if (this->m_Health > 0 && !m_invulnerable)
	{
		this->m_Health = this->m_Health - _damage;
		if (this->m_Health <= 0)
		{
			this->m_Health = 0;
			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_SHIELD_DEACTIVATE);

			if( GetRenderNode().GetDissolveFactor() <= 0.0f )
			{
				this->SetIsActive(false);
				this->SetIsAlive(false);
				std::cout << "SHIELD IS DESTROYED!" << std::endl;

//				AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_3D_WEAPONWHOOSH);
			}
		}
	}
}
void CShield::Release ( void )
{
	CBaseEntity::Release();
}

void CShield::ResetShield()
{
	// Default Orientation.
	GetRenderNode().SetUpVector( 0.0f, 1.0f, 0.0f );
	GetRenderNode().SetRightVector( 1.0f, 0.0f, 0.0f );
	GetRenderNode().SetForwardVector( 0.0f, 0.0f, 1.0f );

	GetRenderNode().SetDissolveFactor( 1.0f );
	m_InvulnerableTimer		= 0.0f;
	m_LaserTimer			= 0.0f;
	m_ineffectiveHitTimer	= 0.0f;
	m_effectiveHitTimer		= 0.0f;
	m_ineffective			= false;
	m_effective				= false;

	InitShaders();
	this->GetRenderNode().SetColor(0.0f,1.0f,0.2f,0.4f);
}
