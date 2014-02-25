/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : Player.h
Author : Evan Wright

Description : The player! THis is controlled directly by 
			  the user, can die, collide and render. 

Created :  07/16/2013
Modified : 07/18/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef PLAYER_H_
#define PLAYER_H_

#include "LivingEntity.h"
#include "../Header Files/Weapons.h"

enum Player_State
{
	PS_INVULNERABLE = 0,
	PS_RESPAWN,
	PS_DYING,
	PS_DEAD,
	PS_CHARGING_LASER,
};

class CPlayer : public CLivingEntity
{
	int m_Lives;
	int m_State;
	float m_MoveSpeed;	

	float m_PosXOffset;
	float m_PosYOffset;

	bool m_CanJump;
	bool m_Hurt;
	bool m_LowHP;
	bool m_KillScreen;
	float m_HealthPickup;
	float m_JumpTimer;

	Vec3f m_UpVel;
	Vec3f m_ForwardVel;
	Vec3f m_RightVel;

	//WEAPONS
	CWeapons m_Gun;
	float	 m_PullTimer;
	float	 m_PushTimer;
	float	 m_LowHPTimer;
	float	 m_DissolveTimer;
	bool	 m_isLoaded;
	bool	 m_ResetWave;
	bool	 m_StopAnimSwitching;
	

	// Timers for dying state.
	float	m_DeathTimer;

	Vec3f m_MoveUp;
	Vec3f m_MoveRight;
	Vec3f m_MoveForward;

public:

	CPlayer( void );
	~CPlayer( void ) { Release(); }

	void Init();

	// IEntity interface:
	void Release( void );
	bool Update( float _elapsedTime );
	virtual void HandleReaction( void );

	// Helper
	void TakeDamage( int _damage );
	void GainHealth( int _health );
	void UpdateVelocity( void );

	// For movement
	void MoveRight( float _units );
	void RotateRight( float _units );
	void RotateToPosition( Vec3f target, Matrix3x3f _camera );
	void MoveForward( float _units );

	void ResetMatrix(void);

	// Accessors
	inline int	 GetLives( void )				{ return m_Lives;				}
	inline int	 GetCanJump( void )				{ return m_CanJump;				}
	inline bool  GetCanPull( void )				{ return (m_PullTimer==0.0f);	}
	inline bool  GetCanPush( void )				{ return (m_PushTimer==0.0f);	}
	inline float GetPullTimer( void )			{ return m_PullTimer;			}
	inline float GetPushTimer( void )			{ return m_PushTimer;			}
	inline bool  GetIsLoaded( void )			{ return m_isLoaded;			}
	inline bool  GetResetWave( void )			{ return m_ResetWave;			}
	inline bool  GetStopAnimSwitching( void )	{ return m_StopAnimSwitching;	}
	inline CWeapons& GetGun ( void )			{ return m_Gun;					}
	inline bool	GetKillScreen ( void )			{return m_KillScreen;			}
	
	inline Vec3f* GetMoveUp( void )				{ return &m_MoveUp;				}
	inline Vec3f* GetMoveRight( void )			{ return &m_MoveRight;			}
	inline Vec3f* GetMoveForward( void )		{ return &m_MoveForward;		}

	// Mutators
	inline void SetLives( int _lives )		{ m_Lives = _lives;				}
	inline void SetResetWave( bool _b )		{ m_ResetWave = _b;				}
	inline void HasPulled( void )			{ m_PullTimer = PULL_COOLDOWN;	}
	inline void HasPushed( void )			{ m_PushTimer = PUSH_COOLDOWN;	}
	inline void SetIsLoaded(bool _loaded)	{ m_isLoaded = _loaded;			} 
	inline void SetStopAnimSwitching(bool _b) { m_StopAnimSwitching = _b;	}
	inline void SetKillScreen( bool _kill ) { m_KillScreen = _kill;			}

	inline void SetMoveUp( Vec3f _up )			{ glsCopyVector3(m_MoveUp,_up);			}
	inline void SetMoveRight( Vec3f _right )	{ glsCopyVector3(m_MoveRight,_right);	}
	inline void SetMoveForward( Vec3f _fwd )	{ glsCopyVector3(m_MoveForward,_fwd);	}
};

#endif
