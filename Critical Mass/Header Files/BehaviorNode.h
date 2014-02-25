/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : Node.h
Author : Evan Wright

Description : The nodes that are used to traverse the Behavior Tree. These contain
			  behaviors and conditions, that are all used by calling process.

Created :  08/07/2013
Modified : 08/07/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef _NODE_H_
#define _NODE_H_

#include "Definitions.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Interface for all behaviors and conditions to derive from.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class CEnemy;
class BehaviorNode
{
protected:
	BehaviorReturnCode	state;
public:
	virtual ~BehaviorNode(void) = 0 {}
	virtual void Release(void) = 0 {}

	// Interface:
	virtual BehaviorReturnCode Behave(CEnemy* _enemy, float _elapsedTime) = 0;
	virtual void Reset(void) = 0;
};

#endif