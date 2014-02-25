#ifndef WEAPONS_H_
#define WEAPONS_H_

#include "glsMath.h"

#define X_ANGLE_SHOTGUN_SPREAD 6
#define Y_ANGLE_SHOTGUN_SPREAD 0
#define NUMBER_SHOTGUN_BULLETS 1

class CObjectFactory;
class CProjectile;
class CPlayer;
class CLaser;
class CWeapons
{
	float			m_TotalTime;
	float			m_RateOfFire;
	float			m_ElapsedTime;
	unsigned int	m_Stored;
	unsigned int	m_Consumed;
	float			m_ShotTotal;

	// For Laser: Not Tom.
	float			m_LaserTime;
	float			m_LaserChargeTimer;
	CLaser*			m_LaserPtr;

	// For the weapon level up system.
	// The arrays use the color type as the key.
	int				m_WeaponLevel[3];
	int				m_WeaponExperience[3];
	//bool			m_IsSuperLaserFired;

	//For Shotgun: Tom
	Vec3f m_RandShotgunVel;

public:
	CWeapons(void );
	~CWeapons( void ) { }

	inline void Store(unsigned int _type) { m_Stored = _type; }
	void Process( void );
	bool Shoot( CObjectFactory* _ObjFactory, CPlayer* _player, Vec3f* _cameraTarget );
	void AddTime(float _time);
	void LevelUp(void);

	//Acessors
	inline unsigned int GetStored( void )	{ return m_Stored;		}
	inline unsigned int GetConsumed( void ) { return m_Consumed;	}
	inline float GetShotTotal( void )		{ return m_ShotTotal;	}
	float GetWeaponMaxAmmo( void );
	inline int GetWeaponLevel( int wep = -1 ){ return m_WeaponLevel[wep == -1 ? m_Consumed : wep]; }
	inline int GetExperience( int wep = -1 )	{ return m_WeaponExperience[wep == -1 ? m_Consumed : wep]; }
	unsigned int GetNeededExperience( int wep = -1 );
	//bool GetSuperLaserShot( void )			{ return this->m_IsSuperLaserFired; }

	//Mutators
	void SetShotTotal( float _newTotal )	{ m_ShotTotal = _newTotal;			}
	void SetConsumed( unsigned int _type )	{ m_Consumed = _type;				}
	void SetLaserChargeTimer( float timer ) { m_LaserChargeTimer = timer;		}
	void SetExperience( int _gun, int _exp );
	void SetDefaultBullet( CProjectile* _bullet );
	//void SetSuperLaserFired( bool _laser ) { m_IsSuperLaserFired = _laser; }
	void Reset( void );

	float GetWeaponCD(void) { return (m_RateOfFire <= m_TotalTime) ? 0 : (1.0f - (m_TotalTime / m_RateOfFire)); }

	//For Shotgun: Tom
	float RandShotgunAngle(unsigned int _x_OR_y);
};

#endif
