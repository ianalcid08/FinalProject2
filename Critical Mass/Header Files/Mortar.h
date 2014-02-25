/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : Mortar.h
Author : Evan Wright

Description : The mortar used in RedStrike! It moves at a certain speed and explodes at the given
			  size. Also has a trail, that follows behind it.

Created :  08/22/2013
Modified : 08/22/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef _MORTAR_H_
#define _MORTAR_H_

#include "MovingEntity.h"

class CMortar : public CMovingEntity
{
	bool m_FinalBlast;

public:
	CMortar( void );
	~CMortar( void ) { Release(); }

	void Init( void );
	void Reset( void );
	void Enter( Vec3f _position, bool _finalBlast );

	// IEntity interface:
	void Release( void );
	bool Update( float _elapsedTime );
	virtual void HandleReaction( void );
};

#endif