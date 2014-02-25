/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : LargeEnemy.h
Author : Evan Wright

Description : The large enemy will be the primary melee unit in the game. It will fire
			  a tractor beam at the player, that pulls the the player towards it. When
			  the player gets close enough the large enemy strikes and does massive 
			  damage!

Created :  07/16/2013
Modified : 07/16/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef LARGEENEMY_H_
#define LARGEENEMY_H_

#include "Enemy.h"

class CShield;
class CLargeEnemy : public CEnemy
{
	// Timers for various status'
	float m_StunTimer;
	float m_PushTimer;
	float m_LaserTimer;
	float m_DeathTimer;
	float m_HurtTimer;
	float m_JustSpawnedTimer;

	// For moving that isn't in the same direction as the look.
	Vec3f m_MoveUp;
	Vec3f m_MoveRight;
	Vec3f m_MoveForward;

	// Special things for Large Enemy! :D
	CShield* m_Shield;
	bool	 m_NoShield;

public:
	CLargeEnemy( void );
	~CLargeEnemy( void ) { Release(); }

	// Helpers
	void Init(void);
	void Kill(void);
	void Reset(void);
	void TakeDamage(int _amount);

	// Math Helpers
	void UpdateVelocity( void );
	void TurnTo( Vec3f _target );

	// IEntity interface:
	void Release( void );
	bool Update( float _elapsedTime );
	void HandleReaction( void );

	// Mutators
	void SetShield( CShield* _shield ) {m_Shield = _shield;}

	// Accessors
	CShield* GetShield( void ) {return m_Shield;}
};

#endif