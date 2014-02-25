/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : Mortar.h
Author : Evan Wright

Description : This is the marker on the ground, that calls the mortars to the ground, in it's area.
			  Creates mortars, handles how fast the spawn and how long they spawn.

Created :  08/22/2013
Modified : 08/22/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "../Header Files/stdafx.h"
#include "../Header Files/Mortar.h"
#include "../Header Files/CollisionLib.h"
#include "../Header Files/AssetManager.h"
#include "../Header Files/MessageSystem.h"

//Sound includes...........................................
#include "../../Includes/Sound/Source/Wwise_IDs.h"
//#include "../../Includes/Sound/Source/Entity3D.h"
#include "../../Includes/Sound/Source/AudioSystemWwise.h"
//.........................................................


#define MORTAR_START_DISTANCE	75.0f

#define MORTAR_SPEED_NORMAL		100.0f
#define MORTAR_SPEED_FINAL		30.0f

CMortar::CMortar( void )
{
	this->SetType(ET_MORTAR);

	// default orientation.
	GetRenderNode().SetUpVector( 0.0f, 1.0f, 0.0f );
	GetRenderNode().SetRightVector( 1.0f, 0.0f, 0.0f );
	GetRenderNode().SetForwardVector( 0.0f, 0.0f, 1.0f );

	Vec3f pos = { 0.0f, 0.0f, 0.0f };
	this->SetCollisionEntity(new Sphere(1.0f, pos));
	m_FinalBlast = false;

	Reset();
}

void CMortar::Enter( Vec3f _position, bool _finalBlast )
{
	m_FinalBlast = _finalBlast;

	GetRenderNode().SetPosition( _position );

	// Orient based on planets surface, so it's RESTING on the surface of the planetoid.
	Vec3f planet = { 0.0f, 75.0f, 0.0f };
	Vec3f newUp, newRight, newForward;
	glsSubtractVectors3( newUp, planet, this->GetRenderNode().GetPosition() );
	float length = glsGetVectorLength3( newUp );
	glsNormalizeVector3( newUp );
	GetRenderNode().SetUpVector( newUp );

	// CLAMP TO THE PLANET!
	float off = 0.0f;
	if( length < 75.0f )
		off = length - 75;
	else if(length > 75.0f)
		off = length - 75;
	if(off != 0.0f)
	{
		Vec3f pos, newPos;
		Vec3f move;
		glsCopyVector3( pos, this->GetRenderNode().GetPosition() );
		glsCopyVector3( move, newUp );
		glsScaleVector3( move, off );
		glsAddVectors3( newPos, pos, move );	
		GetRenderNode().SetPosition( newPos );
	}

	glsCrossProduct3( newForward, this->GetRenderNode().GetRightVector(), newUp );
	glsNormalizeVector3( newForward );
	GetRenderNode().SetForwardVector( newForward );

	glsCrossProduct3( newRight, newUp, newForward );
	glsNormalizeVector3( newRight );
	GetRenderNode().SetRightVector( newRight );

	// Move off of the planet.
	// Now move it a bit off the ground, so the base of the mesh is touching the ground.
	Vec3f tempUp, newPos;
	glsCopyVector3(tempUp,this->GetRenderNode().GetUpVector());
	glsScaleVector3(tempUp,-MORTAR_START_DISTANCE);
	glsAddVectors3(newPos,this->GetRenderNode().GetPosition(),tempUp);
	glsCopyVector3(*this->GetPosition(),newPos);
	GetRenderNode().SetPosition( newPos );

	if(m_FinalBlast)
		GetRenderNode().SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[MORTAR_FINALBLAST_TB] );
	else
		GetRenderNode().SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[MORTAR_BASIC_TB] );

//	AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_MORTAR_WHISTLE);
	//AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::SFX_LASER_CHARGE_VARIATION1);
}

void CMortar::Init( void )
{
	GetRenderNode().SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[MORTAR_BASIC_TB] );
	GetRenderNode().SetDiffuseTextureHandle(  CAssetManager::GetInstance()->GetObjData(ET_MORTAR).m_DiffuseHandle );
	
	GetRenderNode().SetColor( 0.8f, 0.8f, 0.8f, 1.0f );

	GetRenderNode().SetSpecularTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_MORTAR).m_hTextureSpecular );
	GetRenderNode().SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[TEX_DIR_LIGHT] );
	GetRenderNode().SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[TEX_DIR_LIGHT] );
	GetRenderNode().SetNormalMatrixUniformHandle( CAssetManager::GetInstance()->m_vUniformNormalHandles[TEX_DIR_LIGHT] );
	GetRenderNode().SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[TEX_DIR_LIGHT] );
	GetRenderNode().SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[TEX_DIR_LIGHT] );
	GetRenderNode().SetTextureUnitHandle1( CAssetManager::GetInstance()->m_vTextureUnit1Handles[TEX_DIR_LIGHT] );
}

void CMortar::Reset( void )
{
	m_FinalBlast = false;
}

void CMortar::Release( void )
{

}

bool CMortar::Update( float _elapsedTime )
{
	// Find what speed to use based on it's type.
	float speed = 0.0f;
	if( m_FinalBlast )
		speed = MORTAR_SPEED_FINAL;
	else
		speed = MORTAR_SPEED_NORMAL;

	Vec3f vel;
	glsCopyVector3(vel, this->GetRenderNode().GetUpVector());
	glsScaleVector3(vel, speed);
	glsCopyVector3(*this->GetVelocity(), vel);
	// Move based on it's up.
	CMovingEntity::Update(_elapsedTime);
	glsCopyVector3(((Sphere*)this->GetCollisionEntity())->m_Center, this->GetRenderNode().GetPosition());
	return true;
}

void CMortar::HandleReaction( void )
{
	std::vector<IEntity*> Col = this->GetCollisions();
	for(std::vector<IEntity*>::iterator iter = Col.begin(); iter != Col.end(); ++iter)
	{
		switch( (*iter)->GetType() )
		{
		case ET_WORLD:
		case ET_PLAYER:
		case ET_SHIELD:
		case ET_SMALLENEMY:
		case ET_MEDIUMENEMY:
		case ET_LARGEENEMY:
			{
				this->SetIsActive(false);
				this->SetIsAlive(false);
				if( m_FinalBlast )
				{
					CMessageSystem::GetInstance()->SendMessageW(new CCreateExplosionMessage(this,ET_MORTAR,false,2.5f));
				}
				else
					CMessageSystem::GetInstance()->SendMessageW(new CCreateExplosionMessage(this,ET_MORTAR,false,1.0f));
			}
			break;
		};
	}
}
