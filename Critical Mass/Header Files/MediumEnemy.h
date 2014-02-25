/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : MediumEnemy.h
Author : Evan Wright

Description : The medium enemy that is the main projectile unit in the game. He fires 
			  mortars and plasma cannons.

Created :  07/16/2013
Modified : 08/14/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef MEDIUMENEMY_H_
#define MEDIUMENEMY_H_

#include "Enemy.h"

class CMediumEnemy : public CEnemy
{
	// Timers for various status'
	float m_StunTimer;
	float m_PushTimer;
	float m_LaserTimer;
	float m_DeathTimer;
	float m_HurtTimer;
	float m_SpawnTimer;

	bool m_PowerUp;
	float m_PowerUpTimer;

	// For moving that isn't in the same direction as the look.
	Vec3f m_MoveUp;
	Vec3f m_MoveRight;
	Vec3f m_MoveForward;

public:
	CMediumEnemy( void );
	~CMediumEnemy( void ) { Release(); }

	void Init(void);
	void Kill(void);
	void Reset(void);

	// Math Helpers
	void UpdateVelocity( void );
	void TurnTo( Vec3f _target );

	// IEntity interface:
	void Release( void );
	bool Update( float _elapsedTime );
	void HandleReaction( void );
	virtual void SwitchAnimation(void);

};

#endif