/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : MovingEntity.cpp
Author : Evan Wright

Description : Basic movable object in the game. Stores all needed data for a 
			  renderable/collidable/moving game object.

Created :  07/16/2013
Modified : 07/19/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "../Header Files/stdafx.h"
#include "../Header Files/MovingEntity.h"

CMovingEntity::CMovingEntity( void )
{
	m_Velocity[0] = 0.0f;
	m_Velocity[1] = 0.0f;
	m_Velocity[2] = 0.0f;
}

// IEntity interface:
void CMovingEntity::Release( void )
{
	CBaseEntity::Release();
}

bool CMovingEntity::Update( float _elapsedTime )
{
	CBaseEntity::Update(_elapsedTime);

	Vec3f* pos = this->GetPosition();
	(*pos)[0] += (*GetVelocity())[0] * _elapsedTime;
	(*pos)[1] += (*GetVelocity())[1] * _elapsedTime;
	(*pos)[2] += (*GetVelocity())[2] * _elapsedTime;
	SetPosition(*pos);
	GetRenderNode().SetPosition( *pos );

	return true;
}

void CMovingEntity::HandleReaction( void )
{

}
