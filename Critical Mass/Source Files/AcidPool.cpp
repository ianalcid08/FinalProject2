/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : AcidPool.cpp
Author : Evan Wright

Description : A HAZARD! Watch out, an acid pool is a dangerous pit
			  of acid! It may not do that much damage right away...
			  But don't stay in it too long or else you'll be
			  disintegrated!

Created :  07/16/2013
Modified : 07/16/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "../Header Files/stdafx.h"
#include "../Header Files/AcidPool.h"

CAcidPool::CAcidPool( void )
{

}

// IEntity interface:
void CAcidPool::Release( void )
{

}

bool CAcidPool::Update( float _elapsedTime )
{
	return true;
}

void CAcidPool::HandleReaction( void )
{

}
