/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : HealthPickup.h
Author : Evan Wright

Description : FINALLY SOMETHING THAT WONT KILL THE PLAYER! This is a health pickup
			  which, you may be able to guess, gives health to the player!
			  The size varies from large to small, large gives a lot and small... Well
			  gives less, but it's still health!

Created :  07/18/2013
Modified : 07/18/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "../Header Files/stdafx.h"
#include "../Header Files/HealthPickup.h"
#include "../Header Files/Definitions.h"
#include "../Header Files/CollisionEntity.h"
#include "../Header Files/CollisionLib.h"
#include "../Header Files/AssetManager.h"
#include "../Header Files/Pull.h"

#include <random>


//Sound includes...........................................
#include "../../Includes/Sound/Source/Wwise_IDs.h"
#include "../../Includes/Sound/Source/Entity3D.h"
#include "../../Includes/Sound/Source/AudioSystemWwise.h"
//.........................................................

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Default constructor
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
CHealthPickup::CHealthPickup( void )
{
	this->SetType(ET_HEALTH_PICKUP_LARGE);
	this->GetRenderNode().SetColor( HEALTHPICKUP_COLOR_R, HEALTHPICKUP_COLOR_G, HEALTHPICKUP_COLOR_B );

	Vec3f posOffset;
	glsLoadVector3( posOffset, 0.0f, 0.0f, 10.0f );
	this->SetCollisionEntity( new Sphere(1.5f, posOffset) );

	// default orientation.
	GetRenderNode().SetUpVector( 0.0f, 1.0f, 0.0f );
	GetRenderNode().SetRightVector( 1.0f, 0.0f, 0.0f );
	GetRenderNode().SetForwardVector( 0.0f, 0.0f, -1.0f );

	glsCopyVector3(m_MoveUp,		this->GetRenderNode().GetUpVector());
	glsCopyVector3(m_MoveRight,		this->GetRenderNode().GetRightVector());
	glsCopyVector3(m_MoveForward,	this->GetRenderNode().GetForwardVector());

	Vec3f tempVel = {0.0f,0.0f,0.0f};
	this->SetVelocity(tempVel);

	//std::cout << "Health Pickup Created!" << std::endl;
}

void CHealthPickup::Initialize( bool _largePickup )
{
	// TODO:: Start healthpickup particle effect
	// TODO:: Healthpickup model.
	Vec3f tempVel = {0.0f,0.0f,0.0f};
	this->SetBeingPulled(false);
	m_PrevPulled = false;
	this->SetVelocity(tempVel);
	// RANDOM POSITION ON PLANET
	// Rotate object.
	Vec3f up;
	Matrix3x3f rotMatrix;
	Vec3f random;
	glsLoadVector3( random, float(rand()%1000),float(rand()%1000),float(rand()%1000) );
	glsNormalizeVector3( random );

	glsRotationMatrix3x3( rotMatrix, float(rand()%900)/10.0f, random[0], random[1], random[2] );
	glsRotateVector( up, this->GetRenderNode().GetUpVector(), rotMatrix );
	glsNormalizeVector3( up );

	Vec3f _moon;
	glsLoadVector3( _moon, 0.0f, 75.0f, 0.0f );

	// Translate based on radius of the sphere. BOOM BANG DONE
	Vec3f final;
	glsScaleVector3( up, 75 );
	//glsSubtractVectors3( final, up, _moon );
	glsAddVectors3( final, up, _moon );
	//this->SetPosition( final );
	GetRenderNode().SetPosition( final );

	glsCopyVector3(((Sphere*)(this->GetCollisionEntity()))->m_Center, final);
	Vec3f id;
	glsLoadVector3( id, 0.0f, 0.0f, 0.0f );
	//this->SetPosition( id );

	if(_largePickup)
		this->SetType(ET_HEALTH_PICKUP_LARGE);
	else
		this->SetType(ET_HEALTH_PICKUP_SMALL);

	// TODO:: Determine if the type should be large or small
	//THIS IS WHY
	GetRenderNode().SetTriangleBatch( &CAssetManager::GetInstance()->m_vTriangleBatches[HEALTHPICKUP_TB] );
	GetRenderNode().SetDiffuseTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_HEALTH_PICKUP_LARGE).m_DiffuseHandle );
	GetRenderNode().SetSpecularTextureHandle( CAssetManager::GetInstance()->GetObjData(ET_HEALTH_PICKUP_LARGE).m_hTextureSpecular );
	GetRenderNode().SetShaderHandle( CAssetManager::GetInstance()->m_vShaderHandles[TEXTURE_ONLY] );
	GetRenderNode().SetMVPUniformHandle( CAssetManager::GetInstance()->m_vUniformMVPHandles[TEXTURE_ONLY] );
	GetRenderNode().SetNormalMatrixUniformHandle( CAssetManager::GetInstance()->m_vUniformNormalHandles[TEXTURE_ONLY] );
	GetRenderNode().SetColorUniformHandle( CAssetManager::GetInstance()->m_vUniformColorHandles[TEXTURE_ONLY] );
	GetRenderNode().SetTextureUnitHandle0( CAssetManager::GetInstance()->m_vTextureUnit0Handles[TEXTURE_ONLY] );
	GetRenderNode().SetTextureUnitHandle1( CAssetManager::GetInstance()->m_vTextureUnit1Handles[TEXTURE_ONLY] );

	GetRenderNode().SetEnableBlending(true);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Stops particle effect and returns it's availability to the objectfactory
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CHealthPickup::Release( void )
{
	// TODO:: Stop particle effect
	// TODO:: Add back to objectfactory
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Updates the animation
Parameters:
[in] _elapsedTime - the time passed since the last frame.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool CHealthPickup::Update( float _elapsedTime )
{
	// TODO:: Update animation
	// if the pickup is being pulled
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

	// CLAMP TO ZE WORLDS
	Vec3f up, right, forward, moon = {0.0f,75.0f,0.0f};
	glsSubtractVectors3( up, moon, this->GetRenderNode().GetPosition() );
	float length = glsGetVectorLength3( up );
	glsNormalizeVector3( up );
	GetRenderNode().SetUpVector( up );

	float rad = 75.0f;
	float off = 0.0f;
	if( length < rad )
		off = length - rad;
	else if(length > rad)
		off = length - rad;

	if(off != 0.0f)
	{
		Vec3f pos;
		Vec3f move;
		glsCopyVector3( pos, *this->GetPosition() );
		glsCopyVector3( move, this->GetRenderNode().GetUpVector() );
		glsScaleVector3( move, off );
		glsAddVectors3( *this->GetPosition(), pos, move );	
	}
	glsCrossProduct3( forward, this->GetRenderNode().GetRightVector(), up );
	glsNormalizeVector3( forward );
	GetRenderNode().SetForwardVector( forward );

	glsCrossProduct3( right, up, forward );
	glsNormalizeVector3( right );
	GetRenderNode().SetRightVector( right );

	m_PrevPulled = m_AmIBeingPulled;
	m_AmIBeingPulled = false;

	this->GetRenderNode().SetPosition((*this->GetPosition())[0],(*this->GetPosition())[1],(*this->GetPosition())[2]);
	glsCopyVector3(((Sphere*)this->GetCollisionEntity())->m_Center, this->GetRenderNode().GetPosition());
	return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Handles the collision reaction.
Parameters:
[in] _other - the entity to react against.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CHealthPickup::HandleReaction( void )
{
	std::vector<IEntity*> Col = this->GetCollisions();
	for(std::vector<IEntity*>::iterator iter = Col.begin(); iter != Col.end(); ++iter)
	{
		// Doesn't do anything, it's only effected by player
		// inside of player.
		if( (*iter)->GetType() == ET_PLAYER )
		{
			this->SetIsActive(false);
			this->SetIsActive(false);
//			AudioSystemWwise::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_SHEILDPICKUP);

			//std::cout << "Health Pickup Destroyed!" << std::endl;
		}
		if((*iter)->GetType() == ET_PULL)
		{
			this->SetBeingPulled(true);
			TurnTo(*((CPull*)(*iter))->GetDestination()->GetPosition());
		}
	}
	Vec3f tempMoon = {0.0f,75.0f,0.0f};
	this->Adjust(tempMoon);
}
void CHealthPickup::UpdateVelocity( void )
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
void CHealthPickup::TurnTo( Vec3f _target )
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