/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : RedBarrel.cpp
Author : Corey Morehead

Description : This Class will hold any variables and set up for the Barrel class.

Created :  08/06/2013
Modified : 08/06/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "../Header Files/stdafx.h"
#include "../Header Files/RedBarrel.h"
#include "../Header Files/Explosion.h"
//#include "../Header Files/Capsule.h" // Not using capsules right now.
#include "../Header Files/CollisionLib.h"
#include "../Header Files/MessageSystem.h"
#include "../Header Files/AssetManager.h"
//SoundManager includes....................................
#include "../../Includes/Sound/Source/AudioSystemWwise.h"
#include "../../Includes/Sound/Source/Wwise_IDs.h"
//.........................................................

CRedBarrel::CRedBarrel( void )
{
	CBaseEntity::SetType(ET_REDBARREL);
	this->m_Health = MAX_HP_BARREL;
	this->m_InvulnerableTimer = 0.0f;
	this->m_LaserTimer = 0.0f;
	this->m_FlashTimer = 0.0f;
	this->m_Explode = false;

	//Collision Check
	Vec3f pos = { 0.0f, 0.0f, 0.0f };
	this->SetCollisionEntity(new Sphere(3.0f,pos));
	Sphere there = *((Sphere*)this->GetCollisionEntity());		//HAX = no capsule to capsule collision

	// Default Orientation.
	GetRenderNode().SetUpVector( 0.0f, 1.0f, 0.0f );
	GetRenderNode().SetRightVector( 1.0f, 0.0f, 0.0f );
	GetRenderNode().SetForwardVector( 0.0f, 0.0f, 1.0f );
}

void CRedBarrel::Initialize ( double _X, double _Y )
{
	// Default Orientation.
	GetRenderNode().SetUpVector( 0.0f, 1.0f, 0.0f );
	GetRenderNode().SetRightVector( 1.0f, 0.0f, 0.0f );
	GetRenderNode().SetForwardVector( 0.0f, 0.0f, 1.0f );

	// Set Active / Alive
	this->SetIsActive(true);
	this->SetIsAlive(true);

	this->m_Health = MAX_HP_BARREL;
	this->m_InvulnerableTimer = 0.0f;
	this->m_LaserTimer = 0.0f;
	this->m_FlashTimer = 0.0f;
	this->m_Explode = false;

	m_rotX = (float)_X;
	m_rotY = (float)_Y;

	// Calculation for the rotation of the Barrel
	Vec3f fwd, up, right, final;
	Matrix3x3f rotMatrixY, rotMatrixX;

	glsRotationMatrix3x3( rotMatrixY, (float)glsDegToRad(_Y), 0.0f, 1.0f, 0.0f );
	glsRotateVector(fwd, this->GetRenderNode().GetForwardVector(), rotMatrixY );
	glsNormalizeVector3(fwd);

	glsRotationMatrix3x3( rotMatrixX, (float)glsDegToRad(_X), fwd[0], fwd[1], fwd[2] );
	glsRotateVector(up, this->GetRenderNode().GetUpVector(), rotMatrixX );
	glsNormalizeVector3(up);

	glsCrossProduct3( right, fwd, up );
	glsNormalizeVector3(right);

	GetRenderNode().SetUpVector( 0.0f, 1.0f, 0.0f );
	GetRenderNode().SetRightVector( 1.0f, 0.0f, 0.0f );
	GetRenderNode().SetForwardVector( 0.0f, 0.0f, 1.0f );

	Vec3f _moon;
	glsLoadVector3( _moon, 0.0f, 75.0f, 0.0f );

	glsScaleVector3( up, -75 );
	glsAddVectors3( final, up, _moon );
	GetRenderNode().SetPosition( final );

	//Collision Check															
	//Vec3f top, topOffset, bottom;
	//glsCopyVector3(bottom, final);											//sets the bottom to the barrel position.
	//glsCopyVector3(topOffset, this->GetRenderNode().m_vUp);					//sets the direction
	//glsScaleVector3(topOffset, 4);											//sets the offset of the barrel
	//glsAddVectors3(top, topOffset, bottom);									//creates the capsule 0,4

	//this->SetCollisionEntity(new Capsule(Segment(top, bottom), 4.0f));		//collision with the capsule

	Vec3f id;
	glsLoadVector3( id, 0.0f, 0.0f, 0.0f );

	// Reorient Ze Matrix
	Vec3f newRight, newUp, newForward;
	glsSubtractVectors3( newUp, _moon, this->GetRenderNode().GetPosition() );
	float length = glsGetVectorLength3( newUp );
	glsNormalizeVector3( newUp );
	GetRenderNode().SetUpVector( newUp );

	/*Vec3f movUp;
	glsCopyVector3(movUp, this->GetRenderNode().m_vUp);
	glsScaleVector3(movUp, 3);
	glsVectorMultiply3(final, final, movUp);
	glsCopyVector3(((Sphere*)(this->GetCollisionEntity()))->m_Center, final);*/	//HAX = no capsule to capsule collision

	glsCopyVector3(((Sphere*)(this->GetCollisionEntity()))->m_Center, *this->GetPosition());

	glsCrossProduct3( newForward, this->GetRenderNode().GetRightVector(), newUp );
	glsNormalizeVector3( newForward );
	GetRenderNode().SetForwardVector( newForward );

	glsCrossProduct3( newRight, newUp, newForward );
	glsNormalizeVector3( newRight );
	GetRenderNode().SetRightVector( newRight );
	
	glsCopyVector3(((Sphere*)this->GetCollisionEntity())->m_Center, *this->GetPosition());
}

void CRedBarrel::InitShaders()
{
	GetRenderNode().SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[BARREL_SHAPE_TB] );
	GetRenderNode().SetDiffuseTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_REDBARREL).m_DiffuseHandle );
	GetRenderNode().SetSpecularTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_REDBARREL).m_hTextureSpecular );
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

void CRedBarrel::TakeDamage (int _damage)
{
	if (this->m_Health > 0)
	{
		this->m_Health = this->m_Health - _damage;		

		if (this->m_Health <= 0)
		{
			//this->m_FlashTimer = 1.5f;
			//TODO: IDEA: FLASH 3 times before EXPLOSION.
			//std::cout << "WATCH OUT!! <-- FlashTimer" << std::endl;

			this->m_Health = 0;
			this->m_Explode = true;
			this->m_Explode = true;
			this->m_Health = 0;
			this->SetIsActive(false);
			this->SetIsAlive(false);
			//CSoundManager::GetInstance()->PlaySound(AK::EVENTS::PLAY_FX_3D_WEAPONWHOOSH);
//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_3D_SMASHCRATE);

		}
	}
}

// IEntity interface:
void CRedBarrel::Release( void )
{
	CBaseEntity::Release();
}

bool CRedBarrel::Update( float _elapsedTime )
{
	CBaseEntity::Update(_elapsedTime);
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
	if( m_FlashTimer > 0.0f )
	{
		m_FlashTimer -= _elapsedTime;
		if( m_FlashTimer < 0.0f )
			m_FlashTimer = 0.0f;
	}

	glsCopyVector3(((Sphere*)this->GetCollisionEntity())->m_Center, this->GetRenderNode().GetPosition());

	return true;
}

void CRedBarrel::HandleReaction( void )
{
	std::vector<IEntity*> Col = this->GetCollisions();
	for(std::vector<IEntity*>::iterator iter = Col.begin(); iter != Col.end(); ++iter)
	{
		switch ((*iter)->GetType())
		{
		case ET_EXPLOSION:
			{
				if( !((CExplosion*)(*iter))->GetIsEMP() )
				{
					TakeDamage(100);
					if(m_Explode)
					{
						//EXPLOSTION
						CMessageSystem::GetInstance()->SendMessage(new CCreateExplosionMessage(this,ET_REDBARREL));
						this->m_Explode = false;
					}
				}
			}
		case ET_BULLET_PLAYER:
			{
				TakeDamage(100);
				if(m_Explode)
				{
					//EXPLOSTION
					CMessageSystem::GetInstance()->SendMessage(new CCreateExplosionMessage(this,ET_REDBARREL));
					this->m_Explode = false;
				}
			}
			break;
		case ET_LASER:
			{
				if(m_LaserTimer == 0.0f)
				{
					this->TakeDamage(20);		//Hard coded for more damage
					m_LaserTimer = 0.1f;
					//EXPLOSTION
					if(m_Explode)
					{
						CMessageSystem::GetInstance()->SendMessage(new CCreateExplosionMessage(this,ET_REDBARREL));
						this->m_Explode = false;
					}
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
