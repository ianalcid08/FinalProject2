/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : LaserTrap.cpp
Author : Evan Wright

Description : A HAZARD! Watch out, this laser trap spins around a central point;
			  damaging anything in it's radius! 

Created :  07/16/2013
Modified : 07/16/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "../Header Files/stdafx.h"
#include "../Header Files/LaserTrap.h"

CLaserTrap::CLaserTrap( void )
{

}

// IEntity interface:
void CLaserTrap::Release( void )
{

}

bool CLaserTrap::Update( float _elapsedTime )
{
	return true;
}

void CLaserTrap::HandleReaction( void )
{

}
