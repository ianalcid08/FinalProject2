/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : EMP.h
Author : Evan Wright

Description : The mortar shot in the game. It should paint a target on the ground that will cause
			  an EMP explosion to happen after a set interval.

Created :  08/15/2013
Modified : 08/15/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef _EMP_H_
#define _EMP_H_

#include "BaseEntity.h"

class CEMP : public CBaseEntity
{
	float m_ImpactTimer;
	float m_StartTimer;
	float m_RotDir;
	Vec3f m_StartPos;
	
	Vec3f m_ColorStart;
	Vec3f m_ColorEnd;

public:
	CEMP( void );
	~CEMP( void );

	void Enter(float _impactTimer, Vec3f _position);

	void Init(void);

	// IEntity interface:
	void Release( void );
	bool Update( float _elapsedTime );
	void HandleReaction( void );

	float GetImpactTimer( void ) const {return m_ImpactTimer;}
	void  SetImpactTimer( float timer ) {m_ImpactTimer = timer;}
};

#endif