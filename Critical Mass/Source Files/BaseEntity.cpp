/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : BaseEntity.cpp
Author : Evan Wright

Description : Basic renderable object in the game. Stores all needed data for a 
			  renderable/collidable game object.

Created :  07/16/2013
Modified : 07/19/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "../Header Files/stdafx.h"
#include "../Header Files/IEntity.h"
#include "../Header Files/BaseEntity.h"
#include "../Header Files/VFXManager.h"

CBaseEntity::CBaseEntity( void )
{

}

// IEntity interface:
void CBaseEntity::Release( void )
{
	delete this->m_CollisionEntity;
	this->m_CollisionEntity = nullptr;
}

bool CBaseEntity::Update( float _elapsedTime )
{
	return true;
}

void CBaseEntity::HandleReaction( void )
{

}

void CBaseEntity::AddCollision(IEntity* _Col)
{
	for(std::vector<IEntity*>::iterator iter = m_Collisions.begin(); iter != m_Collisions.end(); ++iter)
	{
		if((*iter) == _Col)
			return;
	}

	m_Collisions.push_back(_Col);
}