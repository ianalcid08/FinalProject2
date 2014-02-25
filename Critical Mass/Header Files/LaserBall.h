/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
Title : LaserBall.h
Author : Evan Wright

Description : The laser ball used in laser field. It spawns a ball that when started spawns
			  on the ground. It gradually moves up into the air, to a set distance, shooting
			  a laser downward, towards the ground.

Created :  09/21/2013
Modified : 09/21/2013
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef _LASER_BALL_H_
#define _LASER_BALL_H_

#include "MovingEntity.h"

class CLaserBall : public CMovingEntity
{

public:
	CLaserBall(void);
	~CLaserBall(void) {Release();}

	void Init(float*,float*);
	void Adjust(void);

	// IEntity interface:
	virtual void Release(void);
	virtual bool Update(float);
	virtual void HandleReaction(void);
};

#endif