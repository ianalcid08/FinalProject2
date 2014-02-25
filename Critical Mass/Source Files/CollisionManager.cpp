/*/////////////////////////////////////////////////////////
/Filename:		CollisionManager.cpp
/Author:		Tom Stefl & Ryan Cartier
/Date:			8/4/2013
/Last Modified: 8/4/2013
/*/////////////////////////////////////////////////////////
#include "../Header Files/StdAfx.h"
#include "..\Header Files\CollisionManager.h"
#include "../Header Files/IEntity.h"
#include "../Header Files/BaseEntity.h"

CollisionManager::CollisionManager(void)
{
}


CollisionManager::~CollisionManager(void)
{
}

/*//////////////////////////////////////////////
/This function loops through the collision pairs
/and calls their HandleReaction() based on the 
/first object against the second followed by the 
/second against the first.
/*/////////////////////////////////////////////
void CollisionManager::Execute()
{
 	for(std::vector<IEntity*>::iterator iter = m_Collisions.begin(); iter != m_Collisions.end(); ++iter)
	{
		(*iter)->HandleReaction();
		((CBaseEntity*)(*iter))->GetCollisions().clear();
	}

	//when all the collisions are handled clear that list;
	m_Collisions.clear();
}


/*//////////////////////////////////////////////
/This function adds a collision pair to the 
/vector of pairs.
/*/////////////////////////////////////////////
void CollisionManager::AddCollision(IEntity* _a, IEntity* _b)
{
	if(!IsContained(_a))
		m_Collisions.push_back(_a);
	if(!IsContained(_b))
		m_Collisions.push_back(_b);
	
	((CBaseEntity*)_a)->AddCollision(_b);
	((CBaseEntity*)_b)->AddCollision(_a);
}

/*//////////////////////////////////////////////
/This function takes in a pair as a parameter and
/checks to see if the pair is already inside the
/list of collision pairs.
/*/////////////////////////////////////////////
bool CollisionManager::IsContained(IEntity* _a)
{
	for(std::vector<IEntity*>::iterator iter = m_Collisions.begin(); iter != m_Collisions.end(); ++iter)
	{
		if((*iter) == _a)
			return true;
	}

	return false;
}