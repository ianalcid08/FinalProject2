/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : AcidPool.h
Author : Evan Wright

Description : A HAZARD! Watch out, an acid pool is a dangerous pit
			  of acid! It may not do that much damage right away...
			  But don't stay in it too long or else you'll be
			  disintegrated!

Created :  07/16/2013
Modified : 07/16/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef ACIDPOOL_H_
#define ACIDPOOL_H_

#include "BaseEntity.h"

class CAcidPool : public CBaseEntity
{
	int m_Damage;

public:
	CAcidPool( void );
	~CAcidPool( void ) { Release(); }

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