/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : SmallEnemy.h
Author : Evan Wright

Description : The small enemy that will be the main way to gain powerups in our game.
			  He will be a minor challange to the player and is the only enemy that can
			  be pulled into the gun.

Created :  07/16/2013
Modified : 08/12/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef SMALLENEMY_H_
#define SMALLENEMY_H_

#include "Enemy.h"

class CSmallEnemy : public CEnemy
{
	int m_PowerupType;
	int m_Damage;

	bool m_prevIsPulled;
	bool m_IsPulled;
	bool m_IsPushed;

	float m_StunTimer;
	float m_PushTimer;
	float m_ElapsedTime;
	float m_LaserTimer;
	float m_DeathTimer;
	float m_ExploderBlinkTimer;
	float m_SpawnTimer;
	float m_YellowTimer;

	// For moving that isn't in the same direction as the look.
	Vec3f m_MoveUp;
	Vec3f m_MoveRight;
	Vec3f m_MoveForward;

	// For Spawning out of the depot in a direction
	float m_RightDirection;

public:
	CSmallEnemy( void );
	~CSmallEnemy( void ) { Release(); }

	void Init(void);
	void Kill(void);
	void Reset(void);

	// IEntity interface:
	void Release( void );
	bool Update( float _elapsedTime );
	void HandleReaction( void );

	// Math Helpers
	void UpdateVelocity( void );
	void TurnTo( Vec3f _target );

	// Accessors
	int GetDamage( void )	{ return m_Damage; }
	int GetPowerupType( void ) { return m_PowerupType; }

	// Mutators
	void SetDamage( int _damage )	{ this->m_Damage = _damage; }
	void SetPowerupType( int _type ){ this->m_PowerupType = _type; }

};

#endif SMALLENEMY_H_
