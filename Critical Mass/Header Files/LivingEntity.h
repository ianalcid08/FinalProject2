/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : LivingEntity.h
Author : Evan Wright

Description : Basic living, killable, object in the game. Stores all needed data for a 
			  renderable/collidable/moving/killable game object.

Created :  07/16/2013
Modified : 07/19/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef LIVINGENTITY_H_
#define LIVINGENTITY_H_

#include "MovingEntity.h"
#include "AnimationManager.h"

class CLivingEntity : public CMovingEntity
{
	int m_Health;
	// True if the dying animation and needed stuff should be happening.
	bool m_IsDying;
	// When hit invulnerable timer starts, so you don't get destroyed by one collision.
	float m_InvulnerableTime;

	CAnimationManager	m_Anim;

public:
	CLivingEntity( void );
	~CLivingEntity( void ) { Release(); }

	// IEntity interface:
	void Release( void );
	bool Update( float _elapsedTime );
	virtual void HandleReaction( void );

	void Adjust( Vec3f _planet );

	// Accessors
	inline int GetHealth( void ) { return m_Health; }
	inline float GetInvulnerableTime( void ) { return m_InvulnerableTime; }
	inline bool GetIsDying( void ) { return m_IsDying; }
	inline CAnimationManager* GetAnimationManager(void) { return &m_Anim; }
	// Mutators
	inline void SetHealth( int _health ) { m_Health = _health; }
	inline void SetInvulnerableTime( float _iTime ) { m_InvulnerableTime = _iTime; }
	inline void SetIsDying( bool _isDying ) { m_IsDying = _isDying; }
};

#endif