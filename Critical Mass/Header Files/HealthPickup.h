/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : HealthPickup.h
Author : Evan Wright

Description : FINALLY SOMETHING THAT WONT KILL THE PLAYER! This is a health pickup
			  which, you may be able to guess, gives health to the player!
			  The size varies from large to small, large gives a lot and small... Well
			  gives less, but it's still health!

Created :  07/18/2013
Modified : 07/18/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef HEALTHPICKUP_H_
#define HEALTHPICKUP_H_

#include "LivingEntity.h"
#include "../Header Files/glsMath.h"

class CHealthPickup : public CLivingEntity
{
	bool		m_AmIBeingPulled;
	bool		m_PrevPulled;

	//movement
	Vec3f m_MoveUp;
	Vec3f m_MoveRight;
	Vec3f m_MoveForward;

public:
	CHealthPickup( void );
	~CHealthPickup( void ) { Release(); }

	void Initialize( bool _largePickup );

	// IEntity interface:
	void Release( void );
	bool Update( float _elapsedTime );
	void HandleReaction( void );

	void UpdateVelocity( void );
	void TurnTo( Vec3f _target );

	bool GetBeingPulled() {return m_AmIBeingPulled;}
	bool GetPrevPulled() {return m_PrevPulled;}

	void SetBeingPulled(bool _p) { m_AmIBeingPulled = _p;}
	void SetPrevPulled(bool _p) {m_PrevPulled = _p;}
};

#endif