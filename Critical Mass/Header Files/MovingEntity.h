/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : MovingEntity.h
Author : Evan Wright

Description : Basic movable object in the game. Stores all needed data for a 
			  renderable/collidable/moving game object.

Created :  07/16/2013
Modified : 07/19/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef MOVINGENTITY_H_
#define MOVINGENTITY_H_

#include "BaseEntity.h"

class CMovingEntity : public CBaseEntity
{
	Vec3f m_Velocity;
	float m_MaxVelocity;

public:
	CMovingEntity( void );
	inline ~CMovingEntity( void ) { Release(); }

	// IEntity interface:
	void Release( void );
	bool Update( float _elapsedTime );
	virtual void HandleReaction( void );

	// Accessors
	inline Vec3f* GetVelocity( void ) { return &m_Velocity; }
	inline float GetMaxVelocity( void ) { return m_MaxVelocity; }
	// Mutators
	inline void SetVelocity( Vec3f _vel ) 
	{ 
		for( int i = 0; i < 3; ++i )
			m_Velocity[i] = _vel[i];
	}
	inline void SetMaxVelocity( float _maxVelocity ) { m_MaxVelocity = _maxVelocity; }

};

#endif