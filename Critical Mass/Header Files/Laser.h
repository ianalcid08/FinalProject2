/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : Laser.h
Author : Corey Morehead

Description : The is the "laser" blue projectile for the gun. This class will hold the 
information needed to create the caspule needed for collision and the model.

Created :  07/29/2013
Modified : 07/29/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef LASER_H_
#define LASER_H_

#include "BaseEntity.h"

#define MAX_SCALE_TIMER 5.0f


class CObjectFactory;

class CLaser : public CBaseEntity
{
	float			m_LaserTime;
	CBaseEntity*	m_Destination;
	bool			m_beingUsed;

	float			m_LaserScaleTimer;
	float			m_LaserBlastTimer;

	float			m_MaxScaleTimer;

	short			m_CurrLevel;

public :
	CLaser( void );
	~CLaser( void ) { Release(); }
	void Initialize( CBaseEntity* _destination );
	void Release( void );
	bool Update( float _elapsedTime );
	void HandleReaction( void );
	void InitShaders( void );
	void Scale( float u_size, float r_size, float f_size );

	CObjectFactory	*m_ObjectFactory;

	//Accessor
	inline float GetLaserTime( void )		{ return m_LaserTime;		}
	inline float GetLaserScaleTimer( void ) { return m_LaserScaleTimer;	}
	inline short GetCurrLevel(void)			{ return m_CurrLevel; }
	//Mutator
	inline void SetLaserTime( float _time )		{ m_LaserTime = _time;			}
	inline void SetBeingUsed( bool _beingUsed ) { m_beingUsed = _beingUsed;		}
	inline void SetScaleTimer( float _timer )	{ m_LaserScaleTimer = _timer;	}
	inline void SetMaxScaleTimer( float _timer ){ m_MaxScaleTimer = _timer;		}
	inline void SetCurrLevel( short _newShort)  { m_CurrLevel = _newShort; }
};

#endif