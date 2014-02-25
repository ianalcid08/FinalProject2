/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : Sequence.h
Author : Evan Wright

Description : Traverses its child nodes in order, left to right. The Sequence fails as 
			  soon as any child fails. If a child returns "running", then the Sequence 
			  returns "running", and that child will be traversed first on the next tick 
			  (skipping prior children)

Created :  08/08/2013
Modified : 08/09/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef _SEQUENCE_H_
#define _SEQUENCE_H_

#include "BehaviorNode.h"
#include <cstdarg>

class CEnemy;
class Sequence : public BehaviorNode
{
	/// An array of behaviors for the sequence to implement
	BehaviorNode** behaviors;
	/// The size of the array of behaviors
	unsigned int length;
	/// The current index in the array. (used for Running behaviors)
	unsigned int current;
public:
	Sequence(unsigned int _length, ...)
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
	~Sequence(void)
	{
		Release();
	}

	virtual void Release(void)
	{
		if(behaviors != nullptr)
		{
			for(unsigned int i = 0; i < length; ++i)
			{
				if(behaviors[i]!=nullptr)
					SAFE_DELETE(behaviors[i]);
			}
			length = 0;
			current = 0;
			SAFE_DELETE(behaviors);
		}
	}

	virtual BehaviorReturnCode Behave(CEnemy* _enemy, float _elapsedTime)
	{
		while(current < length)
		{
			BehaviorReturnCode result = behaviors[current]->Behave(_enemy,_elapsedTime);
			switch(result)
			{
			case Success:
				current++;
				continue;
			case Running:
				return Running;
			case Failure:
				current = 0;
				return Failure;
			default:
				current = 0;
				return Error;
			}
		}

		current = 0;
		return Success;
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