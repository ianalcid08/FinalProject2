/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : Selector.h
Author : Evan Wright

Description : Traverses its child nodes in order, left to right. The Selector succeeds 
			  as soon as any child succeeds. The Selector fails only if all children fail. 
			  Like Sequence, Selector retains a "running" child and traverses it first on 
			  the next tick.

Created :  08/08/2013
Modified : 08/09/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef _SELECTOR_H_
#define _SELECTOR_H_

#include "BehaviorNode.h"
#include <iostream>
#include <cstdarg>

class CEnemy;
class Selector : public BehaviorNode
{
	/// An array of behaviors for the selector to implement
	BehaviorNode** behaviors;
	/// The size of the array of behaviors
	unsigned int length;
	/// The current index in the array. (used for Running behaviors)
	unsigned int current;
public:
	Selector(unsigned int _length, ...)
		: length(_length), current(0)
	{
		behaviors = new BehaviorNode*[length];
		// Grab all the nodes being sent in.
		va_list args;	
		va_start(args,_length);
		{
			for(unsigned int i = 0; i < length; ++i)
				behaviors[i] = va_arg(args, BehaviorNode*);
		}
		va_end(args);
	}
	~Selector(void)
	{
		Release();
	}

	virtual void Release(void)
	{
		for(unsigned int i = 0; i < length; ++i)
		{
			if(behaviors[i]!=nullptr)
				behaviors[i]->Release();
			SAFE_DELETE(behaviors[i]);
		}
		length = 0;
		current = 0;
		SAFE_DELETE(behaviors);
	}

	virtual BehaviorReturnCode Behave(CEnemy* _enemy, float _elapsedTime)
	{
		while(current < length)
		{		
			BehaviorReturnCode result = behaviors[current]->Behave(_enemy,_elapsedTime);
			switch(result)
			{
			case Success:
				current = 0;
				return Success;
			case Running:
				return Running;
			case Failure:
				current++;
				continue;
			default:
				current = 0;
				return Error;
			}
		}

		current = 0;
		return Failure;
	}

	virtual void Reset(void)
	{
		current = 0;
		while(current < length)
		{
			behaviors[current]->Reset();
			current++;
		}
		current = 0;
	}
};

#endif