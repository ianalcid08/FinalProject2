/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : Push.h
Author : Evan Wright

Description : 

Created :  07/25/2013
Modified : 07/25/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef PUSH_H_
#define PUSH_H_

#include "BaseEntity.h"

class CPush : public CBaseEntity
{
	float			m_DisplayTimer;
	CBaseEntity*	m_Pusher;

public:
	CPush( void );
	~CPush( void );

	void Initialize( CBaseEntity* _pusher );
	void InitShaders();
	// IEntity interface:
	void Release( void );
	bool Update( float _elapsedTime );
	void HandleReaction( void );

	inline CBaseEntity* GetPusher( void ) { return m_Pusher; }
};

#endif