/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : Enemy.h
Author : Evan Wright

Description : Base class for all enemies. Has a target and a current state.

Created :  08/06/2013
Modified : 08/12/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "LivingEntity.h"
#include "BehaviorTree.h"

class CEnemy : public CLivingEntity
{
	BehaviorTree*	m_Tree;
	CBaseEntity*	m_Target;
	unsigned int	m_State;
	char			m_id;
	float			m_MoveSpeed;
	unsigned int	m_Experience;

	bool			m_IsPaused;
	bool			m_LaserDeath;
	int				m_MaxHealth;

public:
	CEnemy(void);
	~CEnemy(void) {Release();}

	// IEntity interface:
	void Release(void);
	bool Update(float _elapsedTime);
	virtual void HandleReaction(void);
	virtual void SwitchAnimation(void) { }

	// Helpers
	void Reorient_UsingForward(void);

	// Accessors
	CBaseEntity*  GetTarget(void)			{return m_Target;}
	unsigned int  GetState(void)			{return m_State;}
	char		  GetID(void)				{return m_id;}
	BehaviorTree* GetTree(void)				{return m_Tree;}
	float		  GetMoveSpeed(void)		{return m_MoveSpeed;}
	bool		  GetIsPaused(void)			{return m_IsPaused;}
	bool		  GetLaserDeath(void)		{return m_LaserDeath;}
	unsigned int  GetExperienceValue(void)	{return m_Experience;}
	int			  GetMaxHealth(void)		{return m_MaxHealth;}

	// Mutators
	void SetTarget(CBaseEntity* _target)			{this->m_Target		= _target;}
	void SetState(unsigned int _state)				{this->m_State		= _state;}
	void SetID(char _id)							{this->m_id			= _id;}
	void SetTree(BehaviorTree* _tree)				{this->m_Tree		= _tree;}
	void SetMoveSpeed(float _moveSpeed)				{this->m_MoveSpeed	= _moveSpeed;}
	void SetIsPaused(bool _isPaused)				{this->m_IsPaused	= _isPaused;}
	void SetLaserDeath(bool _laserDeath)			{this->m_LaserDeath = _laserDeath;}
	void SetExperienceValue(unsigned int _value)	{this->m_Experience = _value;}
	void SetMaxHealth(int _maxHealth)				{this->m_MaxHealth	= _maxHealth;}
};

#endif