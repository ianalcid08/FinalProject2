/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : Projectile.cpp
Author : Corey Morehead

Description : 

Created :  07/16/2013
Modified : 07/16/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "../Header Files/stdafx.h"
#include "../Header Files/Projectile.h"
#include "../Header Files/MessageSystem.h"
#include "../Header Files/AssetManager.h"
#include "../Header Files/CollisionLib.h"
#include "../Header Files/Enemy.h"
#include "../Header Files/Shield.h"

#define BLUE_STRIKE_CHANCE_SMALL	1
#define BLUE_STRIKE_CHANCE_MEDIUM	10
#define BLUE_STRIKE_CHANCE_LARGE	50

CProjectile::CProjectile( void )
{
	CMovingEntity::SetType(ET_BULLET_PLAYER);

	// COLLISION
	Vec3f posOffset;
	glsLoadVector3( posOffset, 0.0f, 0.0f, 0.0f );
	this->SetCollisionEntity(new Sphere(2.0f, posOffset));

	// default orientation.
	GetRenderNode().SetUpVector( 0.0f, 1.0f, 0.0f );
	GetRenderNode().SetRightVector( 1.0f, 0.0f, 0.0f );
	GetRenderNode().SetForwardVector( 0.0f, 0.0f, -1.0f );

	m_Explode = false;
	m_Consumed = false;
	m_FromBoss = false;

	m_Speed = 30.0f;
	m_Strike = false;
	m_TrailTimer = 0.0f;
}

//void CProjectile::Initialize (Vec3f _velocity, Vec3f _position, Matrix4x4f _worldPosition)
void CProjectile::Initialize()
{
	GetRenderNode().SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[MORTAR_BASIC_TB] );
	GetRenderNode().SetDiffuseTextureHandle(  CAssetManager::GetInstance()->GetObjData(ET_SHIELD).m_DiffuseHandle );
	GetRenderNode().SetColor( 1.0f, 1.0f, 0.0f, 1.0f);

	GetRenderNode().SetSpecularTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_BULLET_PLAYER).m_hTextureSpecular );
	GetRenderNode().SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[TEXTURE_ONLY] );
	GetRenderNode().SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[TEXTURE_ONLY] );
	//GetRenderNode().hUniformNormal = CAssetManager::GetInstance()->m_vUniformNormalHandles[TEXTURE_ONLY];
	GetRenderNode().SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[TEXTURE_ONLY] );
	GetRenderNode().SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[TEXTURE_ONLY] );
	//GetRenderNode().hTextureUnit1 = CAssetManager::GetInstance()->m_vTextureUnit1Handles[TEXTURE_ONLY];
}

// IEntity interface:
void CProjectile::Release( void )
{
	CMovingEntity::Release();
}

bool CProjectile::Update( float _elapsedTime )
{
	CBaseEntity::Update(_elapsedTime);
	
	Vec3f* pos = this->GetPosition();
	(*pos)[0] += this->GetRenderNode().GetForwardVector()[0] * m_Speed * _elapsedTime;
	(*pos)[1] += this->GetRenderNode().GetForwardVector()[1] * m_Speed * _elapsedTime;
	(*pos)[2] += this->GetRenderNode().GetForwardVector()[2] * m_Speed * _elapsedTime;
	SetPosition(*pos);
	GetRenderNode().SetPosition( *pos );
	
	Vec3f _moon;
	glsLoadVector3(_moon, 0.0f, 75.0f, 0.0f);
	Adjust(_moon);

	m_life -= _elapsedTime;
	if( m_life <= 0.0f )
	{
		std::cout << "Projectile Destroy \n";
		this->SetIsActive(false);
		this->SetIsAlive(false);
		return false;
	}

	if(/*this->GetType() == ET_BULLET_ENEMY && */ m_TrailTimer >= 0.025f)
	{
		CMessageSystem::GetInstance()->SendMessage(new CCreateVFXMessage(GEO_EMITTER_BULLET_TRAIL,this,0.0f,0.0f,0.0f));
		m_TrailTimer = 0.0f;
	}
	else
	{
		m_TrailTimer += _elapsedTime;
	}


	glsCopyVector3(((Sphere*)this->GetCollisionEntity())->m_Center, this->GetRenderNode().GetPosition());
	return true;
}

void CProjectile::Adjust( Vec3f _planet )
{
	Vec3f newUp, newRight, newForward;
	glsSubtractVectors3( newUp, _planet, (*this->GetPosition()) );
	float length = glsGetVectorLength3( newUp );
	glsNormalizeVector3( newUp );
	GetRenderNode().SetUpVector( newUp );

	if( length > 75.0f || length < 75.0f )
	{
		float off = length - 75.0f;
		Vec3f pos;
		Vec3f move;
		glsCopyVector3( pos, *this->GetPosition() );
		glsCopyVector3( move, newUp );
		glsScaleVector3( move, off );
		glsAddVectors3( *this->GetPosition(), pos, move );
	}

	glsCopyVector3( newForward, *this->GetVelocity() );
	glsNormalizeVector3( newForward );
	
	glsCrossProduct3( newRight, newUp, newForward );
	glsNormalizeVector3( newRight );
	GetRenderNode().SetRightVector( newRight );

	glsCrossProduct3( newForward, newRight, newUp );
	glsNormalizeVector3( newForward );
	GetRenderNode().SetForwardVector( newForward );
};

void CProjectile::HandleReaction( void )
{
	std::vector<IEntity*> Col = this->GetCollisions();
	for(std::vector<IEntity*>::iterator iter = Col.begin(); iter != Col.end(); ++iter)
	{
		if( this->GetType() == ET_BULLET_PLAYER )
		{
			if( (*iter)->GetType() == ET_SMALLENEMY || (*iter)->GetType() == ET_MEDIUMENEMY || (*iter)->GetType() == ET_LARGEENEMY )
			{
				if( ((CEnemy*)(*iter))->GetHealth() > 0.0f )
				{
					this->SetIsActive(false);
					this->SetIsAlive(false);

					bool strike = false;

					if(m_Strike)
					{
						switch((*iter)->GetType())
						{
						case ET_SMALLENEMY:
							strike = (rand()%BLUE_STRIKE_CHANCE_SMALL) == 0;
							break;
						case ET_MEDIUMENEMY:
							strike = (rand()%BLUE_STRIKE_CHANCE_MEDIUM) == 0;
							break;
						case ET_LARGEENEMY:
							strike = (rand()%BLUE_STRIKE_CHANCE_LARGE) == 0;
							break;
						}
					}

					if(this->m_Explode)
					{
						CMessageSystem::GetInstance()->SendMessage(new CCreateExplosionMessage(this,ET_BULLET_PLAYER));
					}
					else if(strike)
					{
						CMessageSystem::GetInstance()->SendMessageW(new CCreateBlueStrikeMessage(((CBaseEntity*)(*iter))->GetRenderNode().GetPosition()[0],
																								 ((CBaseEntity*)(*iter))->GetRenderNode().GetPosition()[1],
																								 ((CBaseEntity*)(*iter))->GetRenderNode().GetPosition()[2]));
					}
				}
			}
			else if( (*iter)->GetType() != ET_PLAYER && (*iter)->GetType() != this->GetType() &&
				(*iter)->GetType() != ET_PULL && (*iter)->GetType() != ET_PUSH &&
				(*iter)->GetType() != ET_WORLD && (*iter)->GetType() != ET_EMP &&
				(*iter)->GetType() != ET_EXPLOSION && (*iter)->GetType() != ET_HEALTH_PICKUP_LARGE &&
				(*iter)->GetType() != ET_HEALTH_PICKUP_SMALL && (*iter)->GetType() != ET_BULLET_ENEMY)
			{
				this->SetIsActive(false);
				this->SetIsAlive(false);

				// Check to see if it should create an 
			}
		}
		else if( this->GetType() == ET_BULLET_ENEMY && this->GetType() != (*iter)->GetType() && (*iter)->GetType() != ET_BULLET_PLAYER)
		{
			if( (*iter)->GetType() == ET_PLAYER )
			{
				this->SetIsActive(false);
				this->SetIsAlive(false);

				// If it hits an object: EXPLODE
				CMessageSystem::GetInstance()->SendMessage(new CCreateExplosionMessage(this,ET_BULLET_ENEMY,true));
			}
			else if( (*iter)->GetType() == ET_SHIELD )
			{
				this->SetIsActive(false);
				this->SetIsAlive(false);
			}
			else if( (*iter)->GetType() == ET_REDBARREL )
			{
				this->SetIsActive(false);
				this->SetIsAlive(false);

				// If it hits an object: EXPLODE
				//if( m_Explode )
				CMessageSystem::GetInstance()->SendMessage(new CCreateExplosionMessage(this,ET_BULLET_ENEMY,true));
			}
		}
	}
}
