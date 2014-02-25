/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : LaserTrap.h
Author : Evan Wright

Description : A HAZARD! Watch out, this laser trap spins around a central point;
			  damaging anything in it's radius! 

Created :  07/16/2013
Modified : 07/16/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef LASERTRAP_H_
#define LASERTRAP_H_

#include "BaseEntity.h"

class CLaserTrap : public CBaseEntity
{
	int m_Damage;

public:
	CLaserTrap( void );
	~CLaserTrap( void ) { Release(); }

	// IEntity interface:
	void Release( void );
	bool Update( float _elapsedTime );
	void HandleReaction( void );

	// Accessors
	inline int GetDamage( void ) { return m_Damage; }
	// Mutators
	inline void SetDamage( int _damage ) { m_Damage = _damage; }
};

#endif