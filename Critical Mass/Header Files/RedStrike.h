/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : RedStrike.h
Author : Evan Wright

Description : This is the marker on the ground, that calls the mortars to the ground, in it's area.
			  Creates mortars, handles how fast the spawn and how long they spawn.

Created :  08/22/2013
Modified : 08/22/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef _RED_STRIKE_H_
#define _RED_STRIKE_H_

#include "BaseEntity.h"

// This is the marker on the ground, that calls the mortars to the ground, in it's area.
// Creates mortars, handles how fast the spawn and how long they spawn.
class CRedStrike : public CBaseEntity
{
	float STRIKE_DURATION;
	float STRIKE_RATE;
	float STRIKE_TIMER_START;
	float STRIKE_TIMER_END;
	float STRIKE_SIZE;

	float m_StrikeDuration;

	float m_Rate;	
	float m_StartTimer;
	float m_EndTimer;
	float m_Timer;
	float m_Size;

	bool m_UseFinalBlast;

	Vec3f m_StartUp;
	Vec3f m_StartRight;
	Vec3f m_StartForward;

public:
	CRedStrike( void );
	~CRedStrike( void ) { Release(); }

	// Helper
	void Enter( Vec3f position, bool _useFinalBlast );
	void Init( void );
	void Reset( float _duration, float _rate, float _timerStart, float _timerEnd, float _strikeSize );
	void Scale( float size );

	// IEntity interface:
	void Release( void );
	bool Update( float _elapsedTime );
	virtual void HandleReaction( void );
};

#endif