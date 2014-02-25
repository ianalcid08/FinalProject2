/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : LivingEntity.cpp
Author : Evan Wright

Description : Basic living, killable, object in the game. Stores all needed data for a 
			  renderable/collidable/moving/killable game object.

Created :  07/16/2013
Modified : 07/19/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "../Header Files/stdafx.h"
#include "../Header Files/LivingEntity.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Default constructor
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
CLivingEntity::CLivingEntity( void )
{
	SetInvulnerableTime(0.0f);
	m_IsDying = false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Destructor
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CLivingEntity::Release( void )
{
	CMovingEntity::Release();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Updates the animation
Parameters:
[in] _elapsedTime - the time passed since the last frame.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool CLivingEntity::Update( float _elapsedTime )
{
	CMovingEntity::Update(_elapsedTime);
	
	// Add gravity code here.
	// Make the player move towards the center of the planet.

	if(GetInvulnerableTime() > 0.0f)
		SetInvulnerableTime(GetInvulnerableTime()-_elapsedTime);

	// Update it's adjustment on the planet's surface.
	Vec3f moon;
	glsLoadVector3( moon, 0.0f, 75.0f, 0.0f );
	Adjust(moon);

	m_Anim.Update(_elapsedTime, &this->GetRenderNode());

	return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Handles the collision reaction.
Parameters:
[in] _other - the entity to react against.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CLivingEntity::HandleReaction( void )
{

}

void CLivingEntity::Adjust( Vec3f _planet )
{
	Vec3f up, right, forward;
	glsSubtractVectors3( up, _planet, this->GetRenderNode().GetPosition() );
	float length = glsGetVectorLength3( up );
	glsNormalizeVector3( up );
	GetRenderNode().SetUpVector( up );

	float rad = 75.0f;

	if(GetType() == ET_SMALLENEMY)
		rad = 78.0f;

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
		glsAddVectors3( pos, pos, move );	
		SetPosition( pos );
		GetRenderNode().SetPosition( pos );
	}

	glsCrossProduct3( forward, this->GetRenderNode().GetRightVector(), up );
	glsNormalizeVector3( forward );
	GetRenderNode().SetForwardVector( forward );

	glsCrossProduct3( right, up, forward );
	glsNormalizeVector3( right );
	GetRenderNode().SetRightVector( right );
};
