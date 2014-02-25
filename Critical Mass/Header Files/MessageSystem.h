/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : MessageSystem.h
Author : Evan Wright

Description : Handles messages based on the message process function sent in.

Created :  08/12/2013
Modified : 08/12/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef _MESSAGE_SYSTEM_H_
#define _MESSAGE_SYSTEM_H_

#include <iostream>
#include <queue>

enum MSG_Type
{
	MSG_CREATE_VFX = 0,
	MSG_FINISH_VFX,
	MSG_CREATE_OBJECT,
	MSG_CREATE_EXPLOSION,
	MSG_CREATE_BULLET,
	MSG_CREATE_SHIELD,
	MSG_CREATE_HEALTH_PICKUP,
	MSG_CREATE_ENEMY,
	MSG_CREATE_EMP,
	MSG_CREATE_MORTAR,
	MSG_CREATE_RED_STRIKE,
	MSG_CREATE_BLUE_STRIKE,
	MSG_CREATE_DEBUG_RENDERNODE,
	MSG_SLOW_TIME,
	MSG_GIVE_EXP,
	MSG_BOSS_PHASE_CHANGE,
	MSG_BOSS_OUTRO,
	MSG_BOSS_INTRO,
	MSG_HUD_LEVEL_UP,
};		

class IMessage
{
	IMessage(const IMessage& message);
	IMessage& operator=(const IMessage& message);
public:
	IMessage(void) {}
	virtual ~IMessage(void) = 0 {}
	virtual int GetMessageID(void) = 0 {}
};

class CMessage : public IMessage
{
	MSG_Type	m_ID;
public:
	CMessage(MSG_Type _id) {m_ID = _id;}
	virtual ~CMessage(void) {}

	virtual int GetMessageID(void) {return m_ID;}
};

class IEntity;
class CCreateVFXMessage : public CMessage
{
	int		 m_VFXType;
	IEntity* m_Target;
	float	 m_OffsetX;
	float	 m_OffsetY;
	float	 m_OffsetZ;
public:
	CCreateVFXMessage(int _vfxType, IEntity* _target, float _x, float _y, float _z) : CMessage(MSG_CREATE_VFX)
	{
		m_VFXType = _vfxType;
		m_Target  = _target;
		m_OffsetX = _x;
		m_OffsetY = _y;
		m_OffsetZ = _z;
	}
	virtual ~CCreateVFXMessage(void) {}

	inline int GetVFXType(void)		const {return m_VFXType;}
	inline IEntity* GetTarget(void)		  {return m_Target;	}
	inline float GetX(void)			const {return m_OffsetX;}
	inline float GetY(void)			const {return m_OffsetY;}
	inline float GetZ(void)			const {return m_OffsetZ;}
};

class CFinishVFXMessage : public CMessage
{
	int m_VFXID;
public:
	CFinishVFXMessage(int _vfxID) : CMessage(MSG_FINISH_VFX)
	{
		m_VFXID = _vfxID;
	}
	virtual ~CFinishVFXMessage(void) {}

	inline int GetVFXID(void) const {return m_VFXID;}
};

class CCreateBulletMessage : public CMessage
{
	IEntity* m_Target;
	float	 m_AngleX;
	float	 m_AngleY;
	bool	 m_Explode;
	bool	 m_IsPlasma;
	bool	 m_IsRandomVelocities;
	bool	 m_Strike;
public:
	CCreateBulletMessage(IEntity* _target, bool _explode,bool _isRandomVel = false, bool _plasma = true, float _angleX = 0.0f, float _angleY = 0.0f,  bool _strike = false) :
	  CMessage(MSG_CREATE_BULLET)
	{
		m_AngleX = _angleX;
		m_AngleY = _angleY;
		m_Explode = _explode;
		m_Target = _target;
		m_IsPlasma = _plasma;
		m_IsRandomVelocities = _isRandomVel;
		m_Strike = _strike;
	}
	~CCreateBulletMessage(void) {}

	inline IEntity* GetTarget(void)			const {return m_Target;				}
	inline float GetAngleX(void)			const {return m_AngleX;				}
	inline float GetAngleY(void)			const {return m_AngleY;				}
	inline bool GetExplode(void)			const {return m_Explode;			}
	inline bool GetIsPlasma(void)			const {return m_IsPlasma;			}
	inline bool GetIsRandVelocities(void)	const {return m_IsRandomVelocities;	}
	inline bool GetStrike(void)				const {return m_Strike;				}
};

class CCreateExplosionMessage : public CMessage
{
	IEntity* m_Target;
	bool	 m_IsEMP;
	unsigned int		 m_Ownership;
	float	 m_Size;
public:
	CCreateExplosionMessage(IEntity* _target,unsigned int _owner, bool _emp = false, float _size = 1.0f) :
	  CMessage(MSG_CREATE_EXPLOSION)
	{
		m_Target = _target;
		m_IsEMP = _emp;
		m_Size = _size;
		m_Ownership = _owner;
	}
	~CCreateExplosionMessage(void) {}

	inline IEntity* GetTarget(void) const {return m_Target;	}
	inline bool GetIsEMP(void)		const {return m_IsEMP;	}
	inline float GetSize(void)		const {return m_Size;	}
	inline unsigned int GetOwner(void) const {return m_Ownership;}
};

class CCreateMortarMessage : public CMessage
{
	float x;
	float y;
	float z;
	bool	 m_FinalBlast;
public:
	CCreateMortarMessage(float _x, float _y, float _z, bool _finalBlast) :
	  CMessage(MSG_CREATE_MORTAR)
	{
		x = _x;
		y = _y;
	  	z = _z;
		m_FinalBlast = _finalBlast;
	}
	~CCreateMortarMessage(void) {}

	inline float GetX(void)			const {return x;			}
	inline float GetY(void)			const {return y;			}
	inline float GetZ(void)			const {return z;			}
	inline bool GetFinalBlast(void) const {return m_FinalBlast;	}
};

class CCreateRedStrikeMessage : public CMessage
{
	IEntity* m_Player;
	bool m_FinalBlast;

	float m_Duration;
	float m_Rate;
	float m_TimerStart;
	float m_TimerEnd;
	float m_Size;

public:
	CCreateRedStrikeMessage(IEntity* _player, bool _finalBlast, float _duration,
							float _rate, float _tStart, float _tEnd, float _size) :
	  CMessage(MSG_CREATE_RED_STRIKE)
	{
		m_Player		= _player;
		m_FinalBlast	= _finalBlast;

		m_Duration		= _duration;
		m_Rate			= _rate;
		m_TimerStart	= _tStart;
		m_TimerEnd		= _tEnd;
		m_Size			= _size;
	}
	~CCreateRedStrikeMessage(void) {}

	inline IEntity* GetPlayer(void) const {return m_Player;		}
	inline bool GetFinalBlast(void) const {return m_FinalBlast;	}
	inline float GetDuration(void)	const {return m_Duration;	}
	inline float GetRate(void)		const {return m_Rate;		}
	inline float GetTimerStart(void)const {return m_TimerStart; }
	inline float GetTimerEnd(void)	const {return m_TimerEnd;	}
	inline float GetSize(void)		const {return m_Size;		}
};

class CCreateBlueStrikeMessage : public CMessage
{
	float x;
	float y;
	float z;
	bool isCrate;
public:
	CCreateBlueStrikeMessage(float _x, float _y, float _z,bool _isCrate = false) :
	  CMessage(MSG_CREATE_BLUE_STRIKE)
	{
		x = _x;
		y = _y;
		z = _z;
		isCrate = _isCrate;
	}
	~CCreateBlueStrikeMessage(void) {}
	inline float GetX(void)			const {return x;		}
	inline float GetY(void)			const {return y;		}
	inline float GetZ(void)			const {return z;		}
	inline bool GetIsCrate(void)	const {return isCrate;	}
};

class CCreateEnemyMessage : public CMessage
{
	IEntity* m_Target;
	int		 m_Type;
public:
	CCreateEnemyMessage(IEntity* _target, int _type) :
	  CMessage(MSG_CREATE_ENEMY)
	{
		m_Target = _target;
		m_Type = _type;
	}
	~CCreateEnemyMessage(void) {}	
	inline IEntity* GetTarget(void) const {return m_Target;	}
	inline int GetType(void)		const {return m_Type;	}
};

class CCreateEMPMessage : public CMessage
{
	float m_ImpactTimer;
	float m_X;
	float m_Y;
	float m_Z;
public:
	CCreateEMPMessage(float _x, float _y, float _z, float _timer=2.0f) :
	  CMessage(MSG_CREATE_EMP)
	{
		m_ImpactTimer = _timer;
		m_X = _x;
		m_Y = _y;
		m_Z = _z;
	}
	~CCreateEMPMessage(void) {}
	inline float GetX(void)				const {return m_X;			}
	inline float GetY(void)				const {return m_Y;			}
	inline float GetZ(void)				const {return m_Z;			}
	inline float GetImpactTimer(void)	const {return m_ImpactTimer;}
};

class CCreateShieldMessage : public CMessage
{
	float m_RotX;
	float m_RotY;
	IEntity* m_Shield;
	bool  m_Invulnerable;
	unsigned int m_Owner;
public:
	CCreateShieldMessage(float _rotX, float _rotY, IEntity* _shield,unsigned int _owner, bool _invulnerable=false) : CMessage(MSG_CREATE_SHIELD)
	{
		m_RotX = _rotX;
		m_RotY = _rotY;
		m_Shield = _shield;
		m_Owner = _owner;
		m_Invulnerable = _invulnerable;
	}
	~CCreateShieldMessage(void) {}
	inline float GetRotX(void)			const {return m_RotX;		 }
	inline float GetRotY(void)			const {return m_RotY;		 }
	inline IEntity* GetShield(void)			  {return m_Shield;		 }
	inline bool GetInvulnerable(void)	const {return m_Invulnerable;}
	inline unsigned int GetOwner(void) const  {return m_Owner;       }
	inline void SetShield(IEntity* _shield)   {m_Shield = _shield;	 }
	inline void SetOwner(unsigned int _owner) {m_Owner = _owner;	 }
};

class CCreateHealthPickupMessage : public CMessage
{
	float m_X;
	float m_Y;
	float m_Z;
public:
	CCreateHealthPickupMessage(float _x, float _y, float _z) : CMessage(MSG_CREATE_HEALTH_PICKUP)
	{
		m_X = _x;
		m_Y = _y;
		m_Z = _z;
	}
	~CCreateHealthPickupMessage(void) {}

	inline float GetX(void) const {return m_X;}
	inline float GetY(void) const {return m_Y;}
	inline float GetZ(void) const {return m_Z;}
};

class CCreateDebugRenderNodeMessage : public CMessage
{
	float m_Pos[3];
	float m_Up[3];
	float m_Right[3];
	float m_Fwd[3];
	float m_Color[4];
public:
	CCreateDebugRenderNodeMessage(float* _pos,float* _up,float* _right,float* _fwd,float* _color) : CMessage(MSG_CREATE_DEBUG_RENDERNODE)
	{
		memcpy(m_Pos,	_pos,	sizeof(float)*3);
		memcpy(m_Up,	_up,	sizeof(float)*3);
		memcpy(m_Right,	_right,	sizeof(float)*3);
		memcpy(m_Fwd,	_fwd,	sizeof(float)*3);
		memcpy(m_Color,	_color,	sizeof(float)*4);
	}
	virtual ~CCreateDebugRenderNodeMessage(void) {}

	inline float* GetPos(void)		{return m_Pos;}
	inline float* GetUp(void)		{return m_Up;}
	inline float* GetRight(void)	{return m_Right;}
	inline float* GetForward(void)	{return m_Fwd;}
	inline float* GetColor(void)	{return m_Color;}
};


class CSlowTimeMessage : public CMessage
{
	float m_SlowTimer;
public:
	CSlowTimeMessage(float _slowTimer) : CMessage(MSG_SLOW_TIME)
	{
		m_SlowTimer = _slowTimer;
	}
	virtual ~CSlowTimeMessage(void) {}

	inline float GetSlowTimer(void) const {return m_SlowTimer;}
};

class CGiveExperienceMessage : public CMessage
{
	int m_Exp;
	unsigned int m_Gun;
public:
	CGiveExperienceMessage(int _exp, unsigned int _gun) : CMessage(MSG_GIVE_EXP)
	{
		m_Exp = _exp;
		m_Gun = _gun;
	}
	virtual ~CGiveExperienceMessage(void) {}
	int GetExp(void)		const	{return m_Exp;}
	unsigned int GetGun(void) const {return m_Gun;}
};

class CBossPhaseChangeMessage : public CMessage
{
	unsigned int m_Phase;
	IEntity* m_Boss;
public:
	CBossPhaseChangeMessage(int _phase, IEntity* _boss) : CMessage(MSG_BOSS_PHASE_CHANGE)
	{
		m_Phase = _phase;
		m_Boss = _boss;
	}
	virtual ~CBossPhaseChangeMessage(void) {}
	inline unsigned int GetPhase(void) const {return m_Phase;}
	inline IEntity* GetBoss(void) {return m_Boss;}
};

class CBossOutroMessage : public CMessage
{
	IEntity* m_Boss;
public:
	CBossOutroMessage(IEntity* _boss) : CMessage(MSG_BOSS_OUTRO)
	{
		m_Boss = _boss;
	}
	virtual ~CBossOutroMessage(void) {}
	inline IEntity* GetBoss(void) {return m_Boss;}
};

class CBossIntroMessage : public CMessage
{
	IEntity* m_Boss;
public:
	CBossIntroMessage(IEntity* _boss) : CMessage(MSG_BOSS_INTRO)
	{
		m_Boss = _boss;
	}
	virtual ~CBossIntroMessage(void) {}
	inline IEntity* GetBoss(void) {return m_Boss;}
};

class CHUDLevelUpMessage : public CMessage
{
	int m_whichWeapon;
	int m_weaponlevel;
	
public:
	CHUDLevelUpMessage(int _whichWeapon, int _newWeaponLevel) : CMessage(MSG_HUD_LEVEL_UP)
	{
		m_whichWeapon = _whichWeapon;
		m_weaponlevel = _newWeaponLevel;
	}
	virtual ~CHUDLevelUpMessage(void) {}

	int GetWeaponLevel() {return m_weaponlevel;}
	int GetWhichWeapon() {return m_whichWeapon;}

};

// Function pointer to the message processor, to use.
typedef void (*MESSAGEPROC)(IMessage*);

class CMessageSystem
{
	// Stores all of the messages.
	std::queue<IMessage*> m_MsgQueue;
	// The pointer to the message processor to use.
	MESSAGEPROC m_MsgProcessor;

	CMessageSystem(void)
		: m_MsgProcessor(nullptr) {}
	CMessageSystem(const CMessageSystem& _messageSystem);
	CMessageSystem& operator=(const CMessageSystem& _messageSystem);
	~CMessageSystem(void) {}
public:
	// Gets handle to the message system
	static CMessageSystem* GetInstance(void);
	// Setup the message system processor, to the sent in function pointer.
	void Initialize(MESSAGEPROC _msgProcessor);
	// Sends a message into the queue.
	void SendMessage(IMessage* _msg);
	// Processes all of the messages in the queue, once per frame, using the function*
	void ProcessMessages(void);
	// Clears out the message queue.
	void ClearMessages(void);
	// Clears any messages that may be left and sets the function* to nullptr.
	void Shutdown(void);
};

#endif
