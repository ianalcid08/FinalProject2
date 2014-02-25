/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : Behaviors.h
Author : Evan Wright

Description : Behaviors for the BehaviorTree to use. These can be anything, that effects
				an entity that has AI. (Enemy)

Created :  08/06/2013
Modified : 08/06/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef _BEHAVIORS_H_
#define _BEHAVIORS_H_

#include "stdafx.h"
#include "Definitions.h"
#include "BehaviorNode.h"
#include <vector>

class CEnemy;
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Chases the entities current target!
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class Movement_Chase : public BehaviorNode
{
public:
	Movement_Chase(void) {}
	virtual void Release(void) {}
	virtual BehaviorReturnCode Behave(CEnemy* _enemy, float _elapsedTime);
	virtual void Reset(void) {}
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Runs away from the entities current target!
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class Movement_Flee : public BehaviorNode
{
public:
	Movement_Flee(void) {}
	virtual void Release(void) {}
	virtual BehaviorReturnCode Behave(CEnemy* _enemy, float _elapsedTime);
	virtual void Reset(void) {}
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Strafes around the target.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class Movement_Strafe : public BehaviorNode
{
public:
	Movement_Strafe(void) {}
	virtual void Release(void) {}
	virtual BehaviorReturnCode Behave(CEnemy* _enemy, float _elapsedTime);
	virtual void Reset(void) {}
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Strafes around the target.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class Movement_Oscillation : public BehaviorNode
{
	float frequency;
	float wavelength;
public:
	Movement_Oscillation(float _frequency, float _wavelength) : frequency(_frequency), wavelength(_wavelength) {}
	virtual void Release(void) {}
	virtual BehaviorReturnCode Behave(CEnemy* _enemy, float _elapsedTime);
	virtual void Reset(void) {}
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Orients the enemy towards it's target.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class Target_LookAt : public BehaviorNode
{
public:
	Target_LookAt(void) {}
	virtual void Release(void) {}
	virtual BehaviorReturnCode Behave(CEnemy* _enemy, float _elapsedTime);
	virtual void Reset(void) {}
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Enemy becomes idle.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class Idle : public BehaviorNode
{
public:
	Idle(void) {}
	virtual void Release(void) {}
	virtual BehaviorReturnCode Behave(CEnemy* _enemy, float _elapsedTime);
	virtual void Reset(void) {}
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Enemy prepares attack
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class Attack_Prepare : public BehaviorNode
{
public:
	Attack_Prepare(void) {}
	virtual void Release(void) {}
	virtual BehaviorReturnCode Behave(CEnemy* _enemy, float _elapsedTime);
	virtual void Reset(void) {}
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Rushes through a target to a position behind set behing them.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class Attack_Rush : public BehaviorNode
{
	bool		m_Start;
	Vec3f		m_RushTarget;
public:
	Attack_Rush(void) {m_Start=true;}
	virtual void Release(void) {m_Start=true;}
	virtual BehaviorReturnCode Behave(CEnemy* _enemy, float _elapsedTime);
	virtual void Reset(void) {m_Start=true;}
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Fires a plasma cannon shot at the target.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class Attack_Shoot_PlasmaCannon : public BehaviorNode
{
	int m_NumShots;
	float m_Angle;
public:
	Attack_Shoot_PlasmaCannon(int _numShots = 1, float _angle = 0.0f) : m_NumShots(_numShots), m_Angle(_angle) {}
	virtual void Release(void) {}
	virtual BehaviorReturnCode Behave(CEnemy* _enemy, float _elapsedTime);
	virtual void Reset(void) {}
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Places three airstrike markers around the target, calling for an EMP!!
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class Attack_Shoot_EMP : public BehaviorNode
{
	unsigned int m_MaxShots;
	unsigned int m_MinShots;
	float m_Radius;
public:
	Attack_Shoot_EMP(unsigned int _maxShots, unsigned int _minShots, float _radius) : m_MaxShots(_maxShots), m_MinShots(_minShots), m_Radius(_radius) {}
	virtual void Release(void) {}
	virtual BehaviorReturnCode Behave(CEnemy* _enemy, float _elapsedTime);
	virtual void Reset(void) {}
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Tracks the enemies movement. At set intervals the enemies previous position will
		 EXPLODE!
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class Attack_ExplosiveWake : public BehaviorNode
{
	struct Wake
	{
		float timer;
		Vec3f pos;
		Wake(float _timer=0.0f,float _x=0.0f,float _y=0.0f,float _z=0.0f)
		{
			timer = _timer;
			pos[0] = _x;
			pos[1] = _y;
			pos[2] = _z;
		}
	};

	struct Path
	{
		std::vector<Wake> m_Wake;

		float m_EndLength;
		float m_WaveLength;
		float m_CurrLength;

		float m_Rate;
		float m_ExplodeTime;

		float m_Angle;

		Vec3f		m_Pos;
		Vec3f		m_PrevPos;
		Vec3f		m_F;
		Vec3f		m_R;
		Vec3f		m_U;

		CEnemy*		m_Temp;

		bool m_Start;

		Path(float _angle,float _currLength=0.0f,float _endLength=1000.0f,float _waveLength=15.0f,
			 float _rate=150.0f,float _explodeTime=0.0f) 
		{
			m_Start			= true;
			m_CurrLength	= _currLength;
			m_EndLength		= _endLength;
			m_WaveLength	= _waveLength;
			m_Rate			= _rate;
			m_ExplodeTime	= _explodeTime;
			m_Angle			= _angle;
		}
	};

	std::vector<Path> m_Paths;
	bool			m_Start;

	float m_StartLength;
	float m_EndLength;
	float m_WaveLength;
	float m_Rate;
	float m_ExplodeTime;
	float m_Angle;
	int	  m_NumWakes;
	
public:
	Attack_ExplosiveWake(int _numWakes=8,float _angle=360.0f,float _currLength=0.0f,float _endLength=235.5f,float _waveLength=15.0f,
						 float _rate=150.0f,float _explodeTime=0.0f) 
	{
		m_Start			= true;
		m_StartLength	= _currLength;
		m_EndLength		= _endLength;
		m_WaveLength	= _waveLength;
		m_Rate			= _rate;
		m_ExplodeTime	= _explodeTime;
		m_Angle			= _angle;
		m_NumWakes		= _numWakes;
	}
	virtual void Release(void) {}
	virtual BehaviorReturnCode Behave(CEnemy* _enemy, float _elapsedTime);
	virtual void Reset(void) {m_Start=true;m_Paths.clear();}
	void Clamp(Path& path);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Places # of spheres in front of the enemy that fire lasers straight up into the air.
		 They then move forward, till a set distance, damaging ANYTHING in it's path.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class Attack_LaserField : public BehaviorNode
{
	unsigned int m_NumSpheres;

public:
	Attack_LaserField(void) {}
	virtual void Release(void) {}
	virtual BehaviorReturnCode Behave(CEnemy* _enemy, float _elapsedTime);
	virtual void Reset(void) {}
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Fires mortars at a target, for a set period of time.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class Attack_MortarField : public BehaviorNode
{
	float m_Duration;
	float m_Rate;
	float m_CurrRate;
	float m_CurrTime;

public:
	Attack_MortarField(float _duration, float _rate) : m_Duration(_duration), m_Rate(_rate), m_CurrRate(_rate), m_CurrTime(0.0f) {}
	virtual void Release(void) {}
	virtual BehaviorReturnCode Behave(CEnemy* _enemy, float _elapsedTime);
	virtual void Reset(void) {m_CurrRate=m_Rate;m_CurrTime=0.0f;}
};

#endif
