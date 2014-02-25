/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : Conditions.h
Author : Evan Wright

Description : Conditions for the BehaviorTree to use. These are what decide whether or not
			  an action can happen. Think of them as if statements. If this is true: 
			  bahavior!

Created :  08/06/2013
Modified : 08/09/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef _CONDITIONS_H_
#define _CONDITIONS_H_

#include "Definitions.h"
#include "BehaviorNode.h"
class CEnemy;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Checks to see if the target is in a set radius from the enemy.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class Check_InsideRange : public BehaviorNode
{
	float m_Range;
	float m_MinRange;
	float m_MaxRange;
public:
	Check_InsideRange(float _range, float min=0.0f, float max=0.0f) : 
						m_Range(_range), m_MinRange(_range+min), m_MaxRange(_range+max) {}
	virtual void Release(void) {m_Range=m_MinRange;}
	virtual BehaviorReturnCode Behave(CEnemy* _enemy, float _elapsedTime);
	virtual void Reset(void) {m_Range=m_MinRange;}
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Checks to see if the target is out of a set radius from the enemy.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class Check_OutsideRange : public BehaviorNode
{
	float m_Range;
	float m_MinRange;
	float m_MaxRange;
public:
	Check_OutsideRange(float _range, float min=0.0f, float max=0.0f) : 
						m_Range(_range), m_MinRange(_range+min), m_MaxRange(_range+max) {}
	virtual void Release(void) {m_Range=m_MinRange;}
	virtual BehaviorReturnCode Behave(CEnemy* _enemy, float _elapsedTime);
	virtual void Reset(void) {m_Range=m_MinRange;}
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Checks to see if a timer has reached the cap. Returns true while it's running.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class TimerRunning : public BehaviorNode
{
	float m_Timer;
	float m_EndTime;
public:
	TimerRunning(float _endTime) {m_EndTime = _endTime; m_Timer = 0.0f;}
	virtual void Release(void) {m_Timer=0.0f;}
	virtual BehaviorReturnCode Behave(CEnemy* _enemy, float _elapsedTime);
	virtual void Reset(void) {m_Timer=0.0f;}
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Checks to see if a timer has reached the cap. Returns true when it's done.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class TimerFinished : public BehaviorNode
{
	float m_Timer;
	float m_EndTime;
public:
	TimerFinished(float _endTime) {m_EndTime = _endTime; m_Timer = 0.0f;}
	virtual void Release(void) {m_Timer=0.0f;}
	virtual BehaviorReturnCode Behave(CEnemy* _enemy, float _elapsedTime);
	virtual void Reset(void) {m_Timer=0.0f;}
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Uses a ratio to determine whether to continue or not.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class RatioCheck : public BehaviorNode
{
	int m_Value;
public:
	RatioCheck(int value) : m_Value(value) {}
	virtual void Release(void) {}
	virtual BehaviorReturnCode Behave(CEnemy* _enemy, float _elapsedTime);
	virtual void Reset(void) {}
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Uses a ratio to determine whether to continue or not.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class HealthCheck : public BehaviorNode
{
	float m_PercentOfMax;
public:
	HealthCheck(float percentOfMax) : m_PercentOfMax(percentOfMax) {}
	virtual void Release(void) {}
	virtual BehaviorReturnCode Behave(CEnemy* _enemy, float _elapsedTime);
	virtual void Reset(void) {}
};

#endif