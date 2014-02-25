/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : BlueStrike.h
Author : Evan Wright

Description : 

Created :  08/23/2013
Modified : 08/23/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef _BLUE_STRIKE_H_
#define _BLUE_STRIKE_H_

#include "BaseEntity.h"

#define BLUE_STRIKE_TIMER_FADE	1.0f

class CBlueStrike : public CBaseEntity
{
	float m_FadeTimer;
	bool m_IsCrate;
public:
	CBlueStrike( void );
	~CBlueStrike( void ) { Release(); }

	// Helper
	void Enter( const Vec3f position , bool _isCrate );
	void Init( void );
	void Reset( void );

	// IEntity interface:
	virtual void Release( void );
	virtual bool Update( float _elapsedTime );
	virtual void HandleReaction( void );

	inline void SetIsCrate(bool _IsCrate) { m_IsCrate = _IsCrate;}

	inline bool GetIsCrate(void) { return m_IsCrate;}
};

#endif