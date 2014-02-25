/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : Conditions.cpp
Author : Evan Wright

Description : Conditions for the BehaviorTree to use. These are what decide whether or not
			  an action can happen. Think of them as if statements. If this is true: 
			  bahavior!

Created :  08/06/2013
Modified : 08/07/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "../Header Files/stdafx.h"
#include "../Header Files/Conditions.h"
#include "../Header Files/Enemy.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Checks to see if the target is in a set radius from the enemy.
Parameters:
[in] enemy - the enemy to use the tree on.
Returns:
	True if the target is in range of the enemy, otherwise: false.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
BehaviorReturnCode Check_InsideRange::Behave(CEnemy* _enemy, float _elapsedTime)
{
	// If player is in range return true!
	bool result = ( glsGetDistance3(*_enemy->GetTarget()->GetPosition(),*_enemy->GetPosition()) < m_Range );
	if( result )
	{
		m_Range = m_MaxRange;
		return Success;
	}

	m_Range = m_MinRange;
	return Failure;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Checks to see if the target is out of a set radius from the enemy.
Parameters:
[in] enemy - the enemy to use the tree on.
Returns:
	True if the target is out of range of the enemy, otherwise: false.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
BehaviorReturnCode Check_OutsideRange::Behave(CEnemy* _enemy, float _elapsedTime)
{
	// If player is out of range return true!
	bool result = ( glsGetDistance3(*_enemy->GetTarget()->GetPosition(),*_enemy->GetPosition()) > m_Range );
	if( result )
	{
		m_Range = m_MaxRange;
		return Success;
	}
	
	m_Range = m_MinRange;
	return Failure;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Checks to see if a timer has reached the cap.
Parameters:
[in] enemy - the enemy to use the tree on.
Returns:
	True if the timer is running, otherwise: false.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
BehaviorReturnCode TimerRunning::Behave(CEnemy* _enemy, float _elapsedTime)
{
	if(m_Timer >= m_EndTime)
	{
		m_Timer = 0.0f;
		return Success;
	}
	else
		m_Timer += _elapsedTime;

	return Running;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary:  Checks to see if a timer has reached the cap.
Parameters:
[in] enemy - the enemy to use the tree on.
Returns:
	True if the timer is up, otherwise: false.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
BehaviorReturnCode TimerFinished::Behave(CEnemy* _enemy, float _elapsedTime)
{
	if(m_Timer >= m_EndTime)
	{
		m_Timer = 0.0f;
		return Success;
	}
	else
		m_Timer += _elapsedTime;

	return Failure;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Uses a ratio to determine whether to continue or not.
Parameters:
[in] enemy - the enemy to use the tree on.
Returns:
	True if the timer is up, otherwise: false.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
BehaviorReturnCode RatioCheck::Behave(CEnemy* _enemy, float _elapsedTime)
{
	int value = rand()%m_Value;
	if( value == 0 )
		return Success;

	return Failure;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Uses a ratio to determine whether to continue or not.
Parameters:
[in] enemy - the enemy to use the tree on.
Returns:
	True if the timer is up, otherwise: false.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
BehaviorReturnCode HealthCheck::Behave(CEnemy* _enemy, float _elapsedTime)
{
	float health = (float)_enemy->GetHealth();
	float max_health = (float)_enemy->GetMaxHealth();
	if( health > max_health * m_PercentOfMax )
		return Success;

	return Failure;
}