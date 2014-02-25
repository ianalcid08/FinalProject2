/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : Pull.h
Author : Evan Wright

Description : 

Created :  07/25/2013
Modified : 07/25/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef PULL_H_
#define PULL_H_

#include "BaseEntity.h"

class CSmallEnemy;
class CPull : public CBaseEntity
{
	CBaseEntity*	m_Destination;	// the place to pull towards. (player in most cases. 
									// May use this for tractor beam, later.
	CBaseEntity*	m_Target;		// the target to pull
	float			m_PullRate;		// how fast to pull the target. *for "minigame"
	bool			m_HasPulled;	// If it's already pulled an enemy.

public:
	CPull( void );
	~CPull( void ) { Release(); }

	void Initialize( CBaseEntity* _destination = nullptr, CBaseEntity* _target = nullptr );
	void InitShaders();
	// IEntity interface:
	void Release( void );
	bool Update( float _elapsedTime );
	void HandleReaction( void );

	// Accessor
	inline bool GetHasPulled( void ) { return m_HasPulled; }
	inline CBaseEntity* GetDestination() {return m_Destination;}

	inline void SetHasPulled( bool has ) { m_HasPulled = has; }
};

#endif